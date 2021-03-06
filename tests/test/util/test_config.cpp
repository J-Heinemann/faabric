#include <catch/catch.hpp>

#include <faabric/util/config.h>
#include <faabric/util/environment.h>

using namespace faabric::util;

namespace tests {
    TEST_CASE("Test default system config initialisation", "[util]") {
        SystemConfig conf;
        conf.reset();

        // CI has to override some stuff
        if (conf.hostType == "ci") {
            REQUIRE(conf.redisStateHost == "redis");
            REQUIRE(conf.redisQueueHost == "redis");
        } else {
            REQUIRE(conf.redisStateHost == "localhost");
            REQUIRE(conf.redisQueueHost == "localhost");
            REQUIRE(conf.hostType == "default");
        }

        REQUIRE(conf.cgroupMode == "on");
        REQUIRE(conf.functionStorage == "local");
        REQUIRE(conf.fileserverUrl == "");
        REQUIRE(conf.netNsMode == "off");
        REQUIRE(conf.pythonPreload == "off");
        REQUIRE(conf.captureStdout == "off");
        REQUIRE(conf.stateMode == "inmemory");
        REQUIRE(conf.wasmVm == "wavm");

        REQUIRE(conf.redisPort == "6379");

        REQUIRE(conf.noScheduler == 0);
        REQUIRE(conf.maxNodes == 5);
        REQUIRE(conf.maxInFlightRatio == 1);
        REQUIRE(conf.maxNodesPerFunction == 5);

        REQUIRE(conf.threadMode == "local");
        REQUIRE(conf.ompThreadPoolSize == 0);

        REQUIRE(conf.globalMessageTimeout == 60000);
        REQUIRE(conf.boundTimeout == 30000);
        REQUIRE(conf.unboundTimeout == 300000);
        REQUIRE(conf.chainedCallTimeout == 300000);

        REQUIRE(conf.defaultMpiWorldSize == 5);
    }

    TEST_CASE("Test overriding system config initialisation", "[util]") {
        std::string originalHostType = getSystemConfig().hostType;

        std::string hostType = setEnvVar("HOST_TYPE", "magic");
        std::string funcStorage = setEnvVar("FUNCTION_STORAGE", "foobar");
        std::string fileserver = setEnvVar("FILESERVER_URL", "www.foo.com");
        std::string cgMode = setEnvVar("CGROUP_MODE", "off");
        std::string nsMode = setEnvVar("NETNS_MODE", "on");
        std::string pythonPre = setEnvVar("PYTHON_PRELOAD", "on");
        std::string captureStdout = setEnvVar("CAPTURE_STDOUT", "on");
        std::string stateMode = setEnvVar("STATE_MODE", "foobar");
        std::string wasmVm = setEnvVar("WASM_VM", "blah");

        std::string redisState = setEnvVar("REDIS_STATE_HOST", "not-localhost");
        std::string redisQueue = setEnvVar("REDIS_QUEUE_HOST", "other-host");
        std::string redisPort = setEnvVar("REDIS_PORT", "1234");

        std::string noScheduler = setEnvVar("NO_SCHEDULER", "1");
        std::string threads = setEnvVar("MAX_NODES", "50");
        std::string inFlightRatio = setEnvVar("MAX_IN_FLIGHT_RATIO", "8888");
        std::string workers = setEnvVar("MAX_NODES_PER_FUNCTION", "7777");

        std::string threadMode = setEnvVar("THREAD_MODE", "threadfoo");
        std::string ompPoolSize = setEnvVar("OMP_THREAD_POOL_SIZE", "1234");

        std::string globalTimeout = setEnvVar("GLOBAL_MESSAGE_TIMEOUT", "9876");
        std::string boundTimeout = setEnvVar("BOUND_TIMEOUT", "6666");
        std::string unboundTimeout = setEnvVar("UNBOUND_TIMEOUT", "5555");
        std::string chainedTimeout = setEnvVar("CHAINED_CALL_TIMEOUT", "9999");

        std::string funcDir = setEnvVar("FUNC_DIR", "/tmp/foo");
        std::string objDir = setEnvVar("OBJ_DIR", "/tmp/bar");
        std::string runtimeDir = setEnvVar("RUNTIME_FILES_DIR", "/tmp/rara");
        std::string sharedDir = setEnvVar("SHARED_FILES_DIR", "/tmp/sss");
        std::string sharedStorageDir = setEnvVar("SHARED_FILES_STORAGE_DIR", "/tmp/sss_store");

        std::string mpiSize = setEnvVar("DEFAULT_MPI_WORLD_SIZE", "2468");

        // Create new conf for test
        SystemConfig conf;

        REQUIRE(conf.hostType == "magic");
        REQUIRE(conf.functionStorage == "foobar");
        REQUIRE(conf.fileserverUrl == "www.foo.com");
        REQUIRE(conf.cgroupMode == "off");
        REQUIRE(conf.netNsMode == "on");
        REQUIRE(conf.pythonPreload == "on");
        REQUIRE(conf.captureStdout == "on");
        REQUIRE(conf.stateMode == "foobar");
        REQUIRE(conf.wasmVm == "blah");

        REQUIRE(conf.redisStateHost == "not-localhost");
        REQUIRE(conf.redisQueueHost == "other-host");
        REQUIRE(conf.redisPort == "1234");

        REQUIRE(conf.noScheduler == 1);
        REQUIRE(conf.maxNodes == 50);
        REQUIRE(conf.maxInFlightRatio == 8888);
        REQUIRE(conf.maxNodesPerFunction == 7777);

        REQUIRE(conf.threadMode == "threadfoo");
        REQUIRE(conf.ompThreadPoolSize == 1234);

        REQUIRE(conf.globalMessageTimeout == 9876);
        REQUIRE(conf.boundTimeout == 6666);
        REQUIRE(conf.unboundTimeout == 5555);
        REQUIRE(conf.chainedCallTimeout == 9999);

        REQUIRE(conf.functionDir == "/tmp/foo");
        REQUIRE(conf.objectFileDir == "/tmp/bar");
        REQUIRE(conf.runtimeFilesDir == "/tmp/rara");
        REQUIRE(conf.sharedFilesDir == "/tmp/sss");
        REQUIRE(conf.sharedFilesStorageDir == "/tmp/sss_store");

        REQUIRE(conf.defaultMpiWorldSize == 2468);

        // Be careful with host type
        setEnvVar("HOST_TYPE", originalHostType);

        setEnvVar("FUNCTION_STORAGE", funcStorage);
        setEnvVar("FILESERVER_URL", fileserver);
        setEnvVar("CGROUP_MODE", cgMode);
        setEnvVar("NETNS_MODE", nsMode);
        setEnvVar("PYTHON_PRELOAD", pythonPre);
        setEnvVar("CAPTURE_STDOUT", captureStdout);
        setEnvVar("STATE_MODE", stateMode);
        setEnvVar("WASM_VM", wasmVm);

        setEnvVar("REDIS_STATE_HOST", redisState);
        setEnvVar("REDIS_QUEUE_HOST", redisQueue);
        setEnvVar("REDIS_PORT", redisPort);

        setEnvVar("NO_SCHEDULER", noScheduler);
        setEnvVar("MAX_NODES", threads);
        setEnvVar("MAX_IN_FLIGHT_RATIO", inFlightRatio);
        setEnvVar("MAX_NODES_PER_FUNCTION", workers);

        setEnvVar("THREAD_MODE", threadMode);
        setEnvVar("OMP_THREAD_POOL_SIZE", threadMode);

        setEnvVar("GLOBAL_MESSAGE_TIMEOUT", globalTimeout);
        setEnvVar("BOUND_TIMEOUT", boundTimeout);
        setEnvVar("UNBOUND_TIMEOUT", unboundTimeout);
        setEnvVar("CHAINED_CALL_TIMEOUT", chainedTimeout);

        setEnvVar("FUNC_DIR", funcDir);
        setEnvVar("OBJ_DIR", objDir);
        setEnvVar("RUNTIME_FILES_DIR", runtimeDir);
        setEnvVar("SHARED_FILES_DIR", sharedDir);
        setEnvVar("SHARED_FILES_STORAGE_DIR", sharedStorageDir);

        setEnvVar("DEFAULT_MPI_WORLD_SIZE", mpiSize);
    }

}