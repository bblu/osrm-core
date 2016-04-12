#include "engine/engine.hpp"
#include "engine/engine_config.hpp"
#include "engine/route_parameters.hpp"

//#include "engine/plugins/distance_table.hpp"
#include "engine/plugins/hello_world.hpp"
//#include "engine/plugins/nearest.hpp"
//#include "engine/plugins/timestamp.hpp"
#include "engine/plugins/trip.hpp"
//#include "engine/plugins/viaroute.hpp"
//#include "engine/plugins/match.hpp"
 
#include "engine/datafacade/datafacade_base.hpp"
#include "engine/datafacade/internal_datafacade.hpp"
#include "engine/datafacade/shared_datafacade.hpp"

#include "storage/shared_barriers.hpp"
#include "util/make_unique.hpp"
#include "util/routed_options.hpp"
#include "util/simple_logger.hpp"

#include <boost/assert.hpp>
#include <boost/interprocess/sync/named_condition.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/thread/lock_types.hpp>

#include <algorithm>
#include <fstream>
#include <utility>
#include <vector>

using namespace osrm;
using namespace osrm::engine;

int main(int argc, const char *argv[])
{
	util::LogPolicy::GetInstance().Unmute();

	bool trial_run = false;
	std::string ip_address;
	int ip_port, requested_thread_num;

	EngineConfig config;
	/*
	const unsigned init_result = util::GenerateServerProgramOptions(
		argc, argv, config.server_paths, ip_address, ip_port, requested_thread_num,
		config.use_shared_memory, trial_run, config.max_locations_trip,
		config.max_locations_viaroute, config.max_locations_distance_table,
		config.max_locations_map_matching);
*/
	config.use_shared_memory = false;
	config.server_paths["base"] = "D:/osrm/data/map.osrm";

	std::unique_ptr<storage::SharedBarriers> barrier;
	// base class pointer to the objects
	datafacade::BaseDataFacade<contractor::QueryEdge::EdgeData> *query_data_facade;

	

	if (config.use_shared_memory)
	{
		barrier = util::make_unique<storage::SharedBarriers>();
		query_data_facade = new datafacade::SharedDataFacade<contractor::QueryEdge::EdgeData>();
	}
	else
	{
		// populate base path
		util::populate_base_path(config.server_paths);
		query_data_facade = new datafacade::InternalDataFacade<contractor::QueryEdge::EdgeData>(
			config.server_paths);
	}

	using DataFacade = datafacade::BaseDataFacade<contractor::QueryEdge::EdgeData>;

	// The following plugins handle all requests.
	plugins::HelloWorldPlugin *hello = new plugins::HelloWorldPlugin();
	plugins::RoundTripPlugin<DataFacade> *trip = new plugins::RoundTripPlugin<DataFacade>(query_data_facade, config.max_locations_trip);

	RouteParameters route_parameters;
	util::json::Object json_result;

	osrm::engine::plugins::BasePlugin::Status return_code;

	if (barrier) {
		// Get a shared data lock so that other threads won't update
		// things while the query is running
		boost::shared_lock<boost::shared_mutex> data_lock{
			(static_cast<datafacade::SharedDataFacade<contractor::QueryEdge::EdgeData> *>(
			query_data_facade))->data_mutex };
		return_code = hello->HandleRequest(route_parameters, json_result);
	}
	else {
		return_code = hello->HandleRequest(route_parameters, json_result);
	}

	return 0;
}