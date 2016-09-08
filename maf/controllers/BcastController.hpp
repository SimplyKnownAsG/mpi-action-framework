#include "maf/controllers/Controller.hpp"

namespace maf {

    class BcastController : public Controller {

    public:

        std::shared_ptr<Action> bcast(std::shared_ptr<Action> action=NULL);
    
    private:

        std::shared_ptr<Action> _default_share(std::shared_ptr<Action> action=NULL) override;

    };

}