#ifndef SEARCH_ENGINE_HPP
#define SEARCH_ENGINE_HPP

#include "engine/search_engine_data.hpp"
#include "engine/routing/alternative_path.hpp"
#include "engine/routing/many_to_many.hpp"
#include "engine/routing/map_matching.hpp"
#include "engine/routing/shortest_path.hpp"
#include "engine/routing/direct_shortest_path.hpp"

#include <type_traits>

namespace osrm
{
namespace engine
{

template <class DataFacadeT> class SearchEngine
{
  private:
    DataFacadeT *facade;
    SearchEngineData engine_working_data;

  public:
    routing::ShortestPathRouting<DataFacadeT> shortest_path;
    routing::DirectShortestPathRouting<DataFacadeT> direct_shortest_path;
    routing::AlternativeRouting<DataFacadeT> alternative_path;
    routing::ManyToManyRouting<DataFacadeT> distance_table;
    routing::MapMatching<DataFacadeT> map_matching;

    explicit SearchEngine(DataFacadeT *facade)
        : facade(facade), shortest_path(facade, engine_working_data),
          direct_shortest_path(facade, engine_working_data),
          alternative_path(facade, engine_working_data),
          distance_table(facade, engine_working_data), map_matching(facade, engine_working_data)
    {
        static_assert(!std::is_pointer<DataFacadeT>::value, "don't instantiate with ptr type");
        static_assert(std::is_object<DataFacadeT>::value,
                      "don't instantiate with void, function, or reference");
    }

    ~SearchEngine() {}
};
}
}

#endif // SEARCH_ENGINE_HPP
