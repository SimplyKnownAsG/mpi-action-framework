#include "mpi.h"

#include "maf/controllers/ScatterController.hpp"
#include "maf/archives/ReadArchive.hpp"
#include "maf/archives/WriteArchive.hpp"
#include "maf/actions/ActionFactory.hpp"

#include "maf/Log.hpp"

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
        mpi_print("ScatterController::scatter(", actions.size(), ")");
        std::vector<int> send_counts(this->size);
        std::vector<int> displacements(this->size);
        // int send_count = 0;
        int displacement = 0;
        std::shared_ptr<Archive> write_archive = std::shared_ptr<Archive>(new WriteArchive);
        size_t index = 0;
        for (auto action : actions) {
            displacements[index] = displacement;
            std::string type_name = action->type_name();
            (*write_archive) & type_name;
            action->serialize(write_archive);
            int current_disp = write_archive->length();
            send_counts[index++] = current_disp - displacement;
            mpi_print("ScatterController::scatter() send_counts.back()=", send_counts.back(), "   ", displacements.back());
            displacement = current_disp;
        }
        std::string content = write_archive->str();
        const char *send_buffer = content.c_str();
        // MPI_Scatter((void*)(send_counts.data()), 1, MPI_INT,
        //             (void*)&recv_count, 1, MPI_INT,
        //             0, MPI_COMM_WORLD);
        MPI_Bcast((void*)(send_counts.data()), this->size, MPI_INT,
                    0, MPI_COMM_WORLD);
        int recv_count = send_counts[this->rank];
        MPI_Bcast((void*)(displacements.data()), this->size, MPI_INT,
                    0, MPI_COMM_WORLD);
        mpi_print("ScatterController::scatter() ... recv_count=", recv_count, " total size=", content.size());
        char *recv_buffer = new char[recv_count];
        MPI_Scatterv((void*)send_buffer, send_counts.data(), displacements.data(), MPI_CHAR,
                    (void*)(&recv_buffer), recv_count, MPI_CHAR,
                    0, MPI_COMM_WORLD);
        mpi_print("ScatterController::scatter() ... recv_buffer=...");
        std::string s(recv_buffer, recv_count);
        std::shared_ptr<Archive> read_archive = std::shared_ptr<Archive>(new ReadArchive(s));
        mpi_print("ScatterController::scatter() ... post ReadArchive");
        return ActionFactory::Create(read_archive);
    }

    void ScatterController::run() {
        mpi_print("ScatterController::run with ", this->_queue.size(), " actions");
        if (this->_queue.empty() && this->rank != 0) {
            this->start();
        }
        else {
            // while (!this->_queue.empty()) {
            //     std::vector<std::shared_ptr<Action>> actions;
            // }
            this->_stop();
        }
    }

    std::string ScatterController::type_name() {
        return "ScatterController";
    }

    std::shared_ptr<Action> ScatterController::_wait() {
        std::vector<std::shared_ptr<Action>> empty_actions;
        return this->scatter(empty_actions);
        // mpi_print("ScatterController::_wait()");
        // int recv_count;
        // // MPI_Scatter((void*)NULL, 1, MPI_INT,
        // //             (void*)&recv_count, 1, MPI_INT,
        // //             0, MPI_COMM_WORLD);
        // std::vector<int> send_counts, displacements;
        // send_counts.resize(this->size);
        // MPI_Bcast((void*)(send_counts.data()), this->size, MPI_INT,
        //           0, MPI_COMM_WORLD);
        // displacements.resize(this->size);
        // MPI_Bcast((void*)(displacements.data()), this->size, MPI_INT,
        //           0, MPI_COMM_WORLD);
        // mpi_print("ScatterController::_wait() ... recv_count=", recv_count);
        // char *recv_buffer = new char[recv_count];
        // for (int ii = 0; ii < recv_count; ii++) {
        //     mpi_print(" recv_buffer[", ii, "]=", recv_buffer[ii]);
        // }
        // MPI_Scatterv((void*)NULL, send_counts.data(), send_counts.data(), MPI_CHAR,
        //             (void*)&recv_buffer, recv_count, MPI_CHAR,
        //             0, MPI_COMM_WORLD);
        // for (int ii; ii < recv_count; ii++) {
        //     mpi_print(" recv_buffer[", ii, "]=", recv_buffer[ii]);
        // }
        // mpi_print("ScatterController::scatter() ... recv_buffer=...");
        // std::string s(recv_buffer, recv_count);
        // mpi_print("ScatterController::scatter() ... recv_buffer=", s);
        // std::shared_ptr<Archive> read_archive = std::shared_ptr<Archive>(new ReadArchive(recv_buffer));
        // return ActionFactory::Create(read_archive);
    }

    void ScatterController::_stop() {
        mpi_print("ScatterController::_stop()");
        std::vector<std::shared_ptr<Action>> actions;
        actions.push_back(ActionFactory::Create("EndLoopAction"));
        actions.push_back(ActionFactory::Create("BcastController"));
        actions.push_back(ActionFactory::Create("ScatterController"));
        // auto end_act = ActionFactory::Create("EndLoopAction");
        // for (int ii = 0; ii < this->size; ii++) {
        //     actions.push_back(end_act);
        // }
        auto end_act = this->scatter(actions);
        end_act->run(); // throws an EndLoopAction exception to successfully terminate .start()
    }

}
