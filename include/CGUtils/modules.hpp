//
// Created by wyz on 2021/7/5.
//

#ifndef CGUTILS_MODULES_HPP
#define CGUTILS_MODULES_HPP

namespace mod{
    namespace __inner__{
        class Inner{

        };
        namespace __exported__{
            class Exported{

            };
        }
        using namespace __exported__;

    }
    using namespace __inner__::__exported__;
}
namespace mod{
    namespace __inner__{
        class MInner{

        };
        namespace __exported__{
            class MExport{

            };
        }

    }
}



#endif //CGUTILS_MODULES_HPP
