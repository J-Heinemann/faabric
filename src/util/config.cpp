#include "config.h"
#include "environment.h"
#include "logging.h"
#include "locks.h"

#include <faabric/util/network.h>


namespace faabric::util {
    SystemConfig &getSystemConfig() {
        static SystemConfig conf;
        return conf;
    }

    SystemConfig::SystemConfig() {
        this->initialise();
    }

    void SystemConfig::initialise() {
        // System
        hostType = getEnvVar("HOST_TYPE", "default");
        functionStorage = getEnvVar("FUNCTION_STORAGE", "local");
        fileserverUrl = getEnvVar("FILESERVER_URL", "");
        serialisation = getEnvVar("SERIALISATION", "json");
        cgroupMode = getEnvVar("CGROUP_MODE", "on");
        netNsMode = getEnvVar("NETNS_MODE", "off");
        logLevel = getEnvVar("LOG_LEVEL", "info");
        pythonPreload = getEnvVar("PYTHON_PRELOAD", "off");
        captureStdout = getEnvVar("CAPTURE_STDOUT", "off");
        stateMode = getEnvVar("STATE_MODE", "inmemory");
        wasmVm = getEnvVar("WASM_VM", "wavm");

        // Redis
        redisStateHost = getEnvVar("REDIS_STATE_HOST", "localhost");
        redisQueueHost = getEnvVar("REDIS_QUEUE_HOST", "localhost");
        redisPort = getEnvVar("REDIS_PORT", "6379");

        // Scheduling
        noScheduler = this->getSystemConfIntParam("NO_SCHEDULER", "0");
        maxNodes = this->getSystemConfIntParam("MAX_NODES", "5");
        maxInFlightRatio = this->getSystemConfIntParam("MAX_IN_FLIGHT_RATIO", "1");
        maxNodesPerFunction = this->getSystemConfIntParam("MAX_NODES_PER_FUNCTION", "5");

        // Threading
        threadMode = getEnvVar("THREAD_MODE", "local");
        ompThreadPoolSize = this->getSystemConfIntParam("OMP_THREAD_POOL_SIZE", "0");

        // Worker-related timeouts (all in seconds)
        globalMessageTimeout = this->getSystemConfIntParam("GLOBAL_MESSAGE_TIMEOUT", "60000");
        boundTimeout = this->getSystemConfIntParam("BOUND_TIMEOUT", "30000");
        unboundTimeout = this->getSystemConfIntParam("UNBOUND_TIMEOUT", "300000");
        chainedCallTimeout = this->getSystemConfIntParam("CHAINED_CALL_TIMEOUT", "300000");

        // Filesystem storage
        functionDir = getEnvVar("FUNC_DIR", "/usr/local/code/faasm/wasm");
        objectFileDir = getEnvVar("OBJ_DIR", "/usr/local/faasm/object");
        runtimeFilesDir = getEnvVar("RUNTIME_FILES_DIR", "/usr/local/faasm/runtime_root");
        sharedFilesDir = getEnvVar("SHARED_FILES_DIR", "/usr/local/faasm/shared");
        sharedFilesStorageDir = getEnvVar("SHARED_FILES_STORAGE_DIR", "/usr/local/faasm/shared_store");

        // MPI
        defaultMpiWorldSize = this->getSystemConfIntParam("DEFAULT_MPI_WORLD_SIZE", "5");

        // Endpoint
        endpointInterface = getEnvVar("ENDPOINT_INTERFACE", "");
        endpointHost = getEnvVar("ENDPOINT_HOST", "");
        endpointPort = this->getSystemConfIntParam("ENDPOINT_PORT", "8080");
        endpointNumThreads = this->getSystemConfIntParam("ENDPOINT_NUM_THREADS", "4");

        if (endpointHost.empty()) {
            // Get the IP for this host
            endpointHost = faabric::util::getPrimaryIPForThisHost(endpointInterface);
        }
    }

    int SystemConfig::getSystemConfIntParam(const char *name, const char *defaultValue) {
        int value = stoi(getEnvVar(name, defaultValue));

        return value;
    };

    void SystemConfig::reset() {
        this->initialise();
    }

    void SystemConfig::print() {
        const std::shared_ptr<spdlog::logger> &logger = getLogger();

        logger->info("--- System ---");
        logger->info("HOST_TYPE                  {}", hostType);
        logger->info("FUNCTION_STORAGE           {}", functionStorage);
        logger->info("FILESERVER_URL             {}", fileserverUrl);
        logger->info("SERIALISATION              {}", serialisation);
        logger->info("CGROUP_MODE                {}", cgroupMode);
        logger->info("NETNS_MODE                 {}", netNsMode);
        logger->info("LOG_LEVEL                  {}", logLevel);
        logger->info("PYTHON_PRELOAD             {}", pythonPreload);
        logger->info("CAPTURE_STDOUT             {}", captureStdout);
        logger->info("STATE_MODE                 {}", stateMode);
        logger->info("WASM_VM                    {}", wasmVm);

        logger->info("--- Redis ---");
        logger->info("REDIS_STATE_HOST           {}", redisStateHost);
        logger->info("REDIS_QUEUE_HOST           {}", redisQueueHost);
        logger->info("REDIS_PORT                 {}", redisPort);

        logger->info("--- Scheduling ---");
        logger->info("NO_SCHEDULER               {}", noScheduler);
        logger->info("MAX_NODES               {}", maxNodes);
        logger->info("MAX_IN_FLIGHT_RATIO        {}", maxInFlightRatio);
        logger->info("MAX_NODES_PER_FUNCTION  {}", maxNodesPerFunction);

        logger->info("--- Threading ---");
        logger->info("THREAD_MODE                {}", threadMode);
        logger->info("OMP_THREAD_POOL_SIZE       {}", ompThreadPoolSize);

        logger->info("--- Timeouts ---");
        logger->info("GLOBAL_MESSAGE_TIMEOUT     {}", globalMessageTimeout);
        logger->info("BOUND_TIMEOUT              {}", boundTimeout);
        logger->info("UNBOUND_TIMEOUT            {}", unboundTimeout);
        logger->info("CHAINED_CALL_TIMEOUT       {}", chainedCallTimeout);

        logger->info("--- Storage ---");
        logger->info("FUNC_DIR                  {}", functionDir);
        logger->info("OBJ_DIR                   {}", objectFileDir);
        logger->info("RUNTIME_FILES_DIR         {}", runtimeFilesDir);
        logger->info("SHARED_FILES_DIR          {}", sharedFilesDir);
        logger->info("SHARED_FILES_STORAGE_DIR  {}", sharedFilesStorageDir);

        logger->info("--- MPI ---");
        logger->info("DEFAULT_MPI_WORLD_SIZE  {}", defaultMpiWorldSize);

        logger->info("--- Endpoint ---");
        logger->info("ENDPOINT_INTERFACE         {}", endpointInterface);
        logger->info("ENDPOINT_HOST              {}", endpointHost);
        logger->info("ENDPOINT_PORT              {}", endpointPort);
        logger->info("ENDPOINT_NUM_THREADS       {}", endpointNumThreads);
    }
}
