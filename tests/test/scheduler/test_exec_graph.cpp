#include <catch/catch.hpp>

#include "faabric_utils.h"

#include <faabric/util/environment.h>
#include <faabric/scheduler/Scheduler.h>
#include <faabric/redis/Redis.h>

using namespace scheduler;


namespace tests {
    TEST_CASE("Test execution graph", "[scheduler]") {
        faabric::Message msgA = faabric::util::messageFactory("demo", "echo");
        faabric::Message msgB1 = faabric::util::messageFactory("demo", "echo");
        faabric::Message msgB2 = faabric::util::messageFactory("demo", "echo");
        faabric::Message msgC1 = faabric::util::messageFactory("demo", "echo");
        faabric::Message msgC2 = faabric::util::messageFactory("demo", "echo");
        faabric::Message msgC3 = faabric::util::messageFactory("demo", "echo");
        faabric::Message msgD = faabric::util::messageFactory("demo", "echo");

        // Set all execution results
        scheduler::Scheduler &sch = scheduler::getScheduler();
        sch.setFunctionResult(msgA);
        sch.setFunctionResult(msgB1);
        sch.setFunctionResult(msgB2);
        sch.setFunctionResult(msgC1);
        sch.setFunctionResult(msgC2);
        sch.setFunctionResult(msgC3);
        sch.setFunctionResult(msgD);

        // Set up chaining relationships
        sch.logChainedFunction(msgA.id(), msgB1.id());
        sch.logChainedFunction(msgA.id(), msgB2.id());
        sch.logChainedFunction(msgB1.id(), msgC1.id());
        sch.logChainedFunction(msgB2.id(), msgC2.id());
        sch.logChainedFunction(msgB2.id(), msgC3.id());
        sch.logChainedFunction(msgC2.id(), msgD.id());

        ExecGraph actual = sch.getFunctionExecGraph(msgA.id());

        ExecGraphNode nodeD = {
                .msg = msgD,
        };

        ExecGraphNode nodeC3 = {
                .msg = msgC3,
        };

        ExecGraphNode nodeC2 = {
                .msg = msgC2,
                .children = {nodeD}
        };

        ExecGraphNode nodeC1 = {
                .msg = msgC1,
        };

        ExecGraphNode nodeB2 = {
                .msg = msgB2,
                .children = {nodeC2, nodeC3}
        };

        ExecGraphNode nodeB1 = {
                .msg = msgB1,
                .children = {nodeC1}
        };

        ExecGraphNode nodeA = {
                .msg = msgA,
                .children = {nodeB1, nodeB2}
        };

        ExecGraph expected{
                .rootNode = nodeA
        };

        // Sense-check nodes in both
        REQUIRE(countExecGraphNodes(actual) == 7);
        REQUIRE(countExecGraphNodes(expected) == 7);

        checkExecGraphEquality(expected, actual);
    }
}
