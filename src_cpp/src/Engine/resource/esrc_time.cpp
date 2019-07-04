/*
 * ======================= esrc_time.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.03
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "esrc_time.h"
#include <memory>


namespace esrc {//------------------ namespace: esrc -------------------------//

namespace time_inn {//-------- namespace: time_inn  --------------//

    std::unique_ptr<TimeBase> timerUPtr;
    std::unique_ptr<TimeCircle> logicTimeCircleUPtr;

}//------------- namespace: time_inn end --------------//

void init_time(){
    time_inn::timerUPtr = std::make_unique<TimeBase>();
    time_inn::logicTimeCircleUPtr = std::make_unique<TimeCircle>( *(time_inn::timerUPtr.get()), 5 );
}

TimeBase &get_timer(){
    return *(time_inn::timerUPtr.get());
}


TimeCircle &get_logicTimeCircle(){
    return *(time_inn::logicTimeCircleUPtr.get());
}




}//---------------------- namespace: esrc -------------------------//


