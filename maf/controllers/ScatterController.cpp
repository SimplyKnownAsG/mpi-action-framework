#include "mpi.h"

#include "maf/controllers/ScatterController.hpp"
#include "maf/archives/ReadArchive.hpp"
#include "maf/archives/WriteArchive.hpp"
#include "maf/actions/ActionFactory.hpp"
#include "maf/actions/SplitMpiAction.hpp"
#include "maf/actions/EmptyAction.hpp"

#include "maf/Log.hpp"

namespace maf {

    ScatterController::ScatterController() : Controller() {
        // empty;
    }

    ScatterController::ScatterController(std::vector<std::shared_ptr<Action>> actions) : Controller(actions) {
        // empty;
    }

    void ScatterController::bcast(std::shared_ptr<Action> action) {
        return;
    }

    void ScatterController::bcast(std::vector<std::shared_ptr<Action>> actions) {
        return;
    }

    void ScatterController::scatter(std::vector<std::shared_ptr<Action>> actions) {
        if (this->rank == 0) {
            this->_populate_queue(actions);
        }

        do {
            std::vector<int> send_counts(this->size);
            std::vector<int> displacements(this->size);
            int displacement = 0;
            std::shared_ptr<Archive> write_archive = std::shared_ptr<Archive>(new WriteArchive);
            size_t index = 0;

            if (this->rank == 0) {
                if (this->_queue.size() < this->size) {
                    int num_actions = this->_queue.size();

                    for (int rank = 0; rank < this->size; rank++) {
                        std::shared_ptr<Action> action(new SplitMpiAction(rank <= num_actions));
                        displacements[index] = displacement;
                        std::string type_name = action->type_name();
                        (*write_archive) & type_name;
                        action->serialize(write_archive);
                        int current_disp = write_archive->length();
                        send_counts[index++] = current_disp - displacement;
                        displacement = current_disp;
                    }
                }

                for (int rank = 0; rank < this->size; rank++) {
                    std::shared_ptr<Action> action;

                    if (this->_queue.empty()) {
                        action = std::shared_ptr<Action>(new EmptyAction);
                    }
                    else {
                        action = this->_queue.front();
                        this->_queue.pop();
                    }

                    displacements[index] = displacement;
                    std::string type_name = action->type_name();
                    (*write_archive) & type_name;
                    action->serialize(write_archive);
                    int current_disp = write_archive->length();
                    send_counts[index++] = current_disp - displacement;
                    displacement = current_disp;
                }
            }

            std::string content = write_archive->str();
            int recv_count;
            MPI_Scatter((void*)(send_counts.data()), 1, MPI_INT,
                        (void*)&recv_count, 1, MPI_INT,
                        0, MPI_COMM_WORLD);
            std::string recv_buffer = std::string(recv_count, ' ');
            MPI_Scatterv((void*)content.data(), send_counts.data(), displacements.data(), MPI_CHAR,
                         (void*)recv_buffer.data(), recv_count, MPI_CHAR,
                         0, MPI_COMM_WORLD);
            std::shared_ptr<Archive> read_archive = std::shared_ptr<Archive>(new ReadArchive(recv_buffer));
            auto action = ActionFactory::Create(read_archive);
            action->start(this->context);
        }
        while (!this->_queue.empty());
    }

    void ScatterController::run() {
        std::vector<std::shared_ptr<Action>> actions; // pass empty and just use the existing queue
        this->scatter(actions);
    }

    std::string ScatterController::type_name() {
        return "ScatterController";
    }

    void ScatterController::_wait() {
        std::vector<std::shared_ptr<Action>> empty_actions;
        this->scatter(empty_actions);
    }

    void ScatterController::_stop() {
        auto end_act = ActionFactory::Create("EndLoopAction");
        std::vector<std::shared_ptr<Action>> actions(this->size, end_act);
        this->scatter(actions);
        end_act->run(); // throws an EndLoopAction exception to successfully terminate .start()
    }

}
