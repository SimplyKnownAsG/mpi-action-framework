#include "maf/controllers/Controller.hpp"

namespace maf {

    class BcastController : Controller {

    public:
   
        virtual Action *get_action();

    };

}