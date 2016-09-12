#include "mpi.h"

#include "maf/controllers/ScatterController.hpp"
#include "maf/archives/ReadArchive.hpp"
#include "maf/archives/WriteArchive.hpp"
#include "maf/actions/ActionFactory.hpp"

namespace maf {

    ScatterController::ScatterController() : Controller() {
        // empty;
    }

    ScatterController::ScatterController(std::vector<std::shared_ptr<Action>> actions) : Controller(actions) {
        // empty;
    }

    std::shared_ptr<Action> ScatterController::bcast(std::shared_ptr<Action> action) {

    }
    
    std::shared_ptr<Action> ScatterController::scatter(std::vector<std::shared_ptr<Action>> actions) {
        std::vector<int> send_counts;
        std::vector<int> displacements;
        int send_count = 0;
        int displacement = 0;
        std::shared_ptr<Archive> write_archive = std::shared_ptr<Archive>(new WriteArchive);
        for (auto action : actions) {
            displacements.push_back(displacement);
            std::string type_name = action->type_name();
            (*write_archive) & type_name;
            action->serialize(write_archive);
            int current_disp = write_archive->length();
            send_counts.push_back(current_disp - displacement);
            displacement = current_disp;
        }
        std::string content = write_archive->str();
        const char *send_buffer = content.c_str();
        int recv_count;
        MPI_Scatter((void*)(send_counts.data()), 1, MPI_INT,
                    (void*)&recv_count, 1, MPI_INT,
                    0, MPI_COMM_WORLD);
        char *recv_buffer = new char[recv_count];
        MPI_Scatterv((void *)send_buffer, send_counts.data(), displacements.data(), MPI_CHAR,
                    (void*)&recv_buffer, recv_count, MPI_CHAR,
                    0, MPI_COMM_WORLD);
        std::shared_ptr<Archive> read_archive = std::shared_ptr<Archive>(new ReadArchive(recv_buffer));
        return ActionFactory::Create(read_archive);
    }

    void ScatterController::run() {
        while (!this->_queue.empty()) {
            std::vector<std::shared_ptr<Action>> actions;
        }
        this->_stop();
    }

    std::string ScatterController::type_name() {
        return "ScatterController";
    }

    std::shared_ptr<Action> ScatterController::_wait() {
        int recv_count;
        MPI_Scatter((void*)NULL, 1, MPI_INT,
                    (void*)&recv_count, 1, MPI_INT,
                    0, MPI_COMM_WORLD);
        char *recv_buffer = new char[recv_count];
        MPI_Scatterv((void *)NULL, (int *)NULL, (int *)NULL, MPI_CHAR,
                    (void*)&recv_buffer, recv_count, MPI_CHAR,
                    0, MPI_COMM_WORLD);
        std::shared_ptr<Archive> read_archive = std::shared_ptr<Archive>(new ReadArchive(recv_buffer));
        return ActionFactory::Create(read_archive);
    }

    void ScatterController::_stop() {
        std::vector<std::shared_ptr<Action>> actions;
        auto end_act = ActionFactory::Create("EndLoopAction");
        for (int ii = 0; ii < this->size; ii++) {
            actions.push_back(end_act);
        }
        this->scatter(actions);
        end_act->run(); // throws an EndLoopAction exception to successfully terminate .start()
    }

}
