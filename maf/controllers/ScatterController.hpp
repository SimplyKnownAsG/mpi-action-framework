#include "maf/controllers/Controller.hpp"

namespace maf {

    class ScatterController : public Controller {

    public:
    
        ScatterController();

        ScatterController(std::vector<std::shared_ptr<Action>> actions);

        std::shared_ptr<Action> bcast(std::shared_ptr<Action> action=NULL);

        std::shared_ptr<Action> scatter(std::vector<std::shared_ptr<Action>> actions);

        virtual void run() override;

        std::string type_name() override;

    private:

        std::shared_ptr<Action> _wait() override;

        void _stop() override;

    };

}
