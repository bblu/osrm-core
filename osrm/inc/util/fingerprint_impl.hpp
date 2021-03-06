#include "util/osrm_exception.hpp"

#include <boost/uuid/name_generator.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <cstring>

#include <algorithm>
#include <string>

#define MD5PREPARE "c059b1a34a045f0fcd4a3161de43a4de"
#define MD5RTREE "620226ff218a93c86e1bbda724a2424b"
#define MD5GRAPH "2f2d8f48ee7aa537fcddd3921dd67052"
#define MD5OBJECTS "1b25903dc46eca0529bcd8c227dcf4b1"

namespace osrm
{
namespace util
{

FingerPrint FingerPrint::GetValid()
{
    FingerPrint fingerprint;

    fingerprint.magic_number = 1297240911;
    fingerprint.md5_prepare[32] = fingerprint.md5_tree[32] = fingerprint.md5_graph[32] = fingerprint.md5_objects[32] = '\0';

    // 6ba7b810-9dad-11d1-80b4-00c04fd430c8 is a Well Known UUID representing the DNS
    // namespace.  Its use here indicates that we own this part of the UUID space
    // in the DNS realm.  *Usually*, we would then generate a UUID based on "something.project-osrm.org",
    // but this whole class is a hack.  Anyway, named_uuid needs to be initialized to something
    // before it can be used, and this is as good as anything else for these purposes.
    fingerprint.named_uuid = boost::uuids::string_generator()( "{6ba7b810-9dad-11d1-80b4-00c04fd430c8}");

    boost::uuids::name_generator gen(fingerprint.named_uuid);
    std::string temp_string;

    std::memcpy(fingerprint.md5_prepare, MD5PREPARE, 32);
    temp_string += fingerprint.md5_prepare;
    std::memcpy(fingerprint.md5_tree, MD5RTREE, 32);
    temp_string += fingerprint.md5_tree;
    std::memcpy(fingerprint.md5_graph, MD5GRAPH, 32);
    temp_string += fingerprint.md5_graph;
    std::memcpy(fingerprint.md5_objects, MD5OBJECTS, 32);
    temp_string += fingerprint.md5_objects;

    fingerprint.named_uuid = gen(temp_string);

    return fingerprint;
}

const boost::uuids::uuid &FingerPrint::GetFingerPrint() const { return named_uuid; }

bool FingerPrint::IsMagicNumberOK(const FingerPrint& other) const { return other.magic_number == magic_number; }

bool FingerPrint::TestGraphUtil(const FingerPrint &other) const
{
    if (!IsMagicNumberOK(other))
    {
        throw exception("hsgr input file misses magic number. Check or reprocess the file");
    }
    return std::equal(md5_graph, md5_graph + 32, other.md5_graph);
}

bool FingerPrint::TestPrepare(const FingerPrint &other) const
{
    if (!IsMagicNumberOK(other))
    {
        throw exception("osrm input file misses magic number. Check or reprocess the file");
    }
    return std::equal(md5_prepare, md5_prepare + 32, other.md5_prepare);
}

bool FingerPrint::TestRTree(const FingerPrint &other) const
{
    if (!IsMagicNumberOK(other))
    {
        throw exception("r-tree input file misses magic number. Check or reprocess the file");
    }
    return std::equal(md5_tree, md5_tree + 32, other.md5_tree);
}

bool FingerPrint::TestQueryObjects(const FingerPrint &other) const
{
    if (!IsMagicNumberOK(other))
    {
        throw exception("missing magic number. Check or reprocess the file");
    }
    return std::equal(md5_objects, md5_objects + 32, other.md5_objects);
}

}
}
