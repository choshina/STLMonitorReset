#include "stdafx.h"
#include <transducer.h>
#include <algorithm>
#include <math.h>

namespace CPSGrader {

    /* Interval robustness */

    // default (static): interval robustness is the same as normal robustness, with TOPs and BOTTOMs at
    // the ends 
    double transducer::compute_lower_rob(){
#ifdef DEBUG__
        printf(">  transducer:computer_lower_rob              IN.\n");
#endif
        compute_robustness();
        if (z.endTime < start_time) {
            z_low.appendSample(start_time, BOTTOM); 
        }
        else
            z_low = z;

         
        //if(trace_data_ptr->back().front() == 28){
        //    cout<<z<<endl;
        //}
#ifdef DEBUG__
        printf( "<  transducer:computer_lower_rob              OUT.\n");
#endif
        return z_low.front().value;
    };

    double transducer::compute_upper_rob(){
#ifdef DEBUG__
        printf( ">  transducer:computer_upper_rob              IN.\n");
#endif
        compute_robustness();
        if (z.endTime < start_time) {
            z_up.appendSample(start_time, BOTTOM); 
        }
        else
            z_up = z;
#ifdef DEBUG__
        printf( "<  transducer:computer_upper_rob              OUT.\n");
#endif
        return z_up.front().value;
    };

    
    double and_transducer::compute_lower_rob(){
#ifdef DEBUG__
        printf( ">  and_transducer:computer_lower_rob           IN.\n");
#endif
        childL->compute_lower_rob();  
        childR->compute_lower_rob();
        z_low.compute_and(childL->z_low,childR->z_low);
        z_low.resize(start_time, min(childL->z_low.endTime,childR->z_low.endTime),BOTTOM);
        if (z_low.empty())
            z_low.appendSample(start_time, BOTTOM);
#ifdef DEBUG__
        printf( "<  and_transducer:computer_lower_rob           OUT.\n");
#endif
        return z_low.front().value;
    };

    double and_transducer::compute_upper_rob(){
#ifdef DEBUG__
        printf( ">  and_transducer:computer_upper_rob           IN.\n");
#endif
        childL->compute_upper_rob();
        childR->compute_upper_rob();
        z_up.compute_and(childL->z_up,childR->z_up);
        z_up.resize(start_time,z_up.endTime,TOP);
        if (z_up.empty())
            z_up.appendSample(start_time,TOP);
#ifdef DEBUG__
        printf( "<  and_transducer:computer_upper_rob           OUT.\n");
#endif
        return z_up.front().value;
    };

    void and_transducer::collect_vio_epoch(vector<double>& vset, double t){
        if (get_zup(t) < 0){
            if(childL->get_zup(t) < 0){
                childL->collect_vio_epoch(vset, t);
            }

            if(childR->get_zup(t) < 0){
                childR->collect_vio_epoch(vset, t);
            }
        }
    }

    void and_transducer::collect_sat_epoch(vector<double>& sset, double t){
        if (get_zlow(t) > 0){
            if(childL->get_zlow(t) > 0){
                childL->collect_sat_epoch(sset, t);
            }

            if(childR->get_zlow(t) > 0){
                childR->collect_sat_epoch(sset, t);
            }
        }
    }

    double and_transducer::min_shift_vio(double t){
        double a = - Signal::BigM;
        double b = - Signal::BigM;
        if (childL->get_zup(t)<0 ){
            a = childL->min_shift_vio(t);
        }
        if (childR->get_zup(t)<0){
            b = childR->min_shift_vio(t);
        }
        return a>b?a:b;
    }

    double and_transducer::min_shift_sat(double t){
        double a = Signal::BigM;
        double b = Signal::BigM;
        if (childL->get_zlow(t)>0){
            a = childL->min_shift_sat(t);
        }
        if(childR->get_zlow(t)>0){
            b = childR->min_shift_sat(t);
        }
        return a<b?a:b;
    }
    
    double or_transducer::compute_lower_rob(){
        childL->compute_lower_rob();
        childR->compute_lower_rob();
        z_low.compute_or(childL->z_low,childR->z_low);
        z_low.resize(start_time,z_low.endTime,BOTTOM);
        if (z_low.empty())
            z_low.appendSample(start_time, BOTTOM);
        return z_low.front().value;
    };

    double or_transducer::compute_upper_rob(){
        childL->compute_upper_rob();
        childR->compute_upper_rob();
        z_up.compute_or(childL->z_up,childR->z_up);
        z_up.resize(start_time,min(childL->z_up.endTime,childR->z_up.endTime),TOP);
        if (z_up.empty())
            z_up.appendSample(start_time,TOP);
		
        return z_up.front().value;
    };

    void or_transducer::collect_vio_epoch(vector<double>& vset, double t){
        if (get_zup(t) < 0){
            if(childL->get_zup(t) < 0){
                childL->collect_vio_epoch(vset, t);
            }

            if(childR->get_zup(t) < 0){
                childR->collect_vio_epoch(vset, t);
            }

        }
    }

    void or_transducer::collect_sat_epoch(vector<double>& sset, double t){
        if (get_zlow(t) > 0){
            if(childL->get_zlow(t) > 0){
                childL->collect_sat_epoch(sset, t);
            }

            if(childR->get_zlow(t) > 0){
                childR->collect_sat_epoch(sset, t);
            }
        }
    }

    double or_transducer::min_shift_vio(double t){
        double a = Signal::BigM;
        double b = Signal::BigM;
        if (childL->get_zup(t)<0 ){
            a = childL->min_shift_vio(t);
        }
        if (childR->get_zup(t)<0){
            b = childR->min_shift_vio(t);
        }
        return a<b?a:b;
    }

    double or_transducer::min_shift_sat(double t){
        double a = - Signal::BigM;
        double b = - Signal::BigM;
        if (childL->get_zlow(t)>0){
            a = childL->min_shift_sat(t);
        }
        if(childR->get_zlow(t)>0){
            b = childR->min_shift_sat(t);
        }
        return a>b?a:b;
    }

// IMPLIES transducer
    double implies_transducer::compute_lower_rob(){
        childL->compute_upper_rob();
        childR->compute_lower_rob();

        Signal z1;
        z1.compute_not(childL->z_up);
        z_low.compute_or(z1,childR->z_low);
        z_low.resize(start_time,z_low.endTime,BOTTOM);

        if (z_low.empty())
            z_low.appendSample(start_time, BOTTOM);
        return z_low.front().value;
    };

    double implies_transducer::compute_upper_rob(){
        childL->compute_lower_rob();
        childR->compute_upper_rob();

        Signal z1;
        z1.compute_not(childL->z_low);
        z_up.compute_or(z1,childR->z_up);
        
        z_up.resize(start_time,min(z1.endTime,childR->z_up.endTime),TOP);
        if (z_up.empty())
            z_up.appendSample(start_time,TOP);
        return z_up.front().value;
    };
    
    // NOT transducer: swap upper and lower
    double not_transducer::compute_upper_rob(){
        child->compute_lower_rob();
        if (child->z_low.empty()) {
            z_up.appendSample(start_time,TOP);
            return TOP;
        }
        z_up.compute_not(child->z_low);
        return z_up.front().value;
    }

    double not_transducer::compute_lower_rob(){
        child->compute_upper_rob();
        if (child->z_up.empty()) {
            z_low.appendSample(start_time,BOTTOM);
            return BOTTOM;
        }
        z_low.compute_not(child->z_up);
        return z_low.front().value;
    }

    void not_transducer::collect_vio_epoch(vector<double>& vset, double t){
        child->collect_sat_epoch(vset, t);
    }

    void not_transducer::collect_sat_epoch(vector<double>& sset, double t){
        child->collect_vio_epoch(sset, t);
    }

    double not_transducer::min_shift_vio(double t){
        return child->min_shift_sat(t);
    }

    double not_transducer::min_shift_sat(double t){
        return child->min_shift_vio(t);
    }

    // EVENTUALLY
    double ev_transducer::compute_lower_rob() {
        // lower robustness for a max operator. Partial information gives a lower bound for max, so we keep it. 

#ifdef DEBUG__
        printf( ">  ev_transducer:computer_lower_rob           IN.\n");
        cout << "   I->a: " << I->begin << "   I->b: " << I->end << endl;
        cout << "   start_time:" << start_time << " end_time:" << end_time << endl;
#endif

        double a,b;
        if (!get_param(I->begin_str,a)) a = I->begin;
        if (!get_param(I->end_str,b)) b = I->end;

        child->compute_lower_rob();
        if (child->z_low.endTime < a) {
            z_low.appendSample(start_time, BOTTOM); 
            return BOTTOM;
        }
    
        z_low.compute_timed_eventually(child->z_low, a, b);
        double et =min(z_low.endTime,end_time);
        z_low.resize(start_time,max(start_time,et), 0.);

        if (z_low.empty()) 
            z_low.appendSample(start_time, BOTTOM); 

#ifdef DEBUG__
        cout << "OUT: z_low:"<< z_low << endl;
        printf( "<  ev_transducer:computer_lower_rob           OUT.\n");
#endif
        return z_low.front().value;
    }

    double ev_transducer::compute_upper_rob() {
        // upper bound on max. Partial info can always be beaten by new samples, so can't say anything. 

#ifdef DEBUG__
        printf( ">  ev_transducer:computer_upper_rob           IN.\n");
        cout << "   I->a: " << I->begin << "   I->b: " << I->end << endl;
        cout << "   start_time:" << start_time << " end_time:" << end_time << endl;
#endif

        double a,b;
        if (!get_param(I->begin_str,a)) a = I->begin;
        if (!get_param(I->end_str,b)) b = I->end;

        child->compute_upper_rob();

        if (child->z_up.endTime < a) {
            z_up.appendSample(start_time, TOP); 
            return TOP;
        }

        z_up.compute_timed_eventually(child->z_up, a, b);

        // Here we remove values computed with partial data 
        double et =min(z_up.endTime-b+a,end_time);
        z_up.resize(start_time,et, 0.);

        if (z_up.empty()) 
            z_up.appendSample(start_time, TOP); 

#ifdef DEBUG__
        cout << "OUT: z_up:"<< z_up << endl;
        printf( "<  ev_transducer:computer_upper_rob           OUT.\n");
#endif
        return z_up.front().value;
    }

    void ev_transducer::collect_vio_epoch(vector<double>& vset, double t){
        if(get_zup(t) < 0){
            for(auto i = child->z_up.begin(); i!= child->z_up.end(); i ++){
                if(child->get_zup((*i).time) < 0){
                    child->collect_vio_epoch(vset, (*i).time);
                }
            }
        }
    }

    void ev_transducer::collect_sat_epoch(vector<double>& sset, double t){
        if(get_zlow(t) > 0){
            for(auto i = child->z_low.begin(); i!= child->z_low.end(); i ++){
                if(child->get_zlow((*i).time) > 0){
                    child->collect_sat_epoch(sset, (*i).time);
                }
            }

        }
    }

    double ev_transducer::min_shift_vio(double t){
        double a = Signal::BigM;
        for(auto i = child->z_up.begin(); i!=child->z_up.end(); i ++){
            if(child->get_zup((*i).time) < 0){
                double b = child->min_shift_vio((*i).time);
                if(b<a){
                    a = b;
                }
            }
        }
        return a;
    }

    double ev_transducer::min_shift_sat(double t){
        double a = - Signal::BigM;
        for(auto i = child->z_low.begin();i!=child->z_low.end();i ++){
            if(child->get_zlow((*i).time)>0){
                double b = child->min_shift_sat((*i).time);
                if(b>a){
                    a = b;
                }
            }
        }
        return a;
    }

    // ALWAYS
    double alw_transducer::compute_lower_rob() {
        // lower bound on a min operator. Partial info cannot help here. 

#ifdef DEBUG__
        printf( ">  alw_transducer:computer_lower_rob          IN.\n");
        cout << "   I->a: " << I->begin << "   I->b: " << I->end << endl;
        cout << "   start_time:" << start_time << " end_time:" << end_time << endl;
#endif

        double a,b;
        if (!get_param(I->begin_str,a)) a = I->begin;
        if (!get_param(I->end_str,b)) b = I->end;

        child->compute_lower_rob();

        if (child->z_low.endTime < a) {
            z_low.appendSample(start_time,BOTTOM);        
            return BOTTOM;
        }
    
        z_low.compute_timed_globally(child->z_low, a, b);

        // Here we remove values computed with partial data 
        double et =min(z_low.endTime-b+a,end_time);
        z_low.resize(start_time,et, 0.);
	
        if (z_low.empty()) 
            z_low.appendSample(start_time,BOTTOM);        

#ifdef DEBUG__
        printf( "OUT: z_low:");
        cout << "<  alw_transducer:computer_lower_rob           OUT."<< endl;
#endif

        return z_low.front().value;
    }

    double alw_transducer::compute_upper_rob() {
#ifdef DEBUG__
        printf( ">  alw_transducer:computer_upper_rob          IN.\n");
        cout << "   I->a: " << I->begin << "   I->b: " << I->end << endl;
        cout << "   start_time:" << start_time << " end_time:" << end_time << endl;
#endif

        double a,b;
        if (!get_param(I->begin_str,a)) a = I->begin;
        if (!get_param(I->end_str,b)) b = I->end;

        child->compute_upper_rob();
        if (child->z_up.endTime < a) {
            z_up.appendSample(start_time, TOP); 
            return TOP;
        }

        //    cout << "child->z_up:" << child->z_up << endl;
        z_up.compute_timed_globally(child->z_up, a, b);
        double et =min(z_up.endTime,end_time);
        z_up.resize(start_time,max(start_time,et), 0.);

        //if(trace_data_ptr->back().front()== 28){
        //    cout<< z << endl << "============" << endl;
        //    cout<< z_up <<endl;
        //}

        if (z_up.empty()) 
            z_up.appendSample(start_time, TOP); 

#ifdef DEBUG__
        cout << "OUT: z_up:"<< z_up << endl;
        printf( "<  alw_transducer:computer_upper_rob          OUT.\n");
#endif
        return z_up.front().value;

    }

    void alw_transducer::collect_vio_epoch(vector<double>& vset, double t){

        //cout<<"zup(t): " << get_zup(t) << "front:" << z_up.front() << endl;
        if(get_zup(t) < 0){               
//                 for(double tp = t+ a; tp <= t + b; tp ++){
//                     if(trace_data_ptr->back().front() == 14 && tp == 11){
//                         cout<<"tp "<< tp<<endl; 
//                         cout<<"ab: " <<a <<"   "<<b <<endl;
//                         cout<< "childzup(tp): "<< child->get_zup(tp)<<" " <<endl;
//                         cout<< "childzup: "<< child->z_up<<endl;
//                     }
// 
//                     if(child->get_zup(tp) < 0){
//                         child->collect_vio_epoch(vset, tp);
//                     }
//                 }
            for(auto i = child->z_up.begin(); i!= child->z_up.end(); i ++){
                if(child->get_zup((*i).time)<0){
                    child->collect_vio_epoch(vset, (*i).time);
                }
            }
        }
    }

    void alw_transducer::collect_sat_epoch(vector<double>& sset, double t){
        if(get_zlow(t) > 0){
            for(auto i = child->z_low.begin(); i!= child->z_low.end(); i ++){
                if(child->get_zlow((*i).time) > 0){
                    child->collect_sat_epoch(sset, (*i).time);
                }
            }
        }
    }

    double alw_transducer::min_shift_vio(double t){
        double a = - Signal::BigM;
        for(auto i = child->z_up.begin(); i!=child->z_up.end(); i ++){
            if(child->get_zup((*i).time) < 0){
                double b = child->min_shift_vio((*i).time);
                if(b>a){
                    a = b;
                }
            }
        }
        return a;
    }

    double alw_transducer::min_shift_sat(double t){
        double a = Signal::BigM;
        for(auto i = child->z_low.begin();i!=child->z_low.end();i ++){
            if(child->get_zlow((*i).time)>0){
                double b = child->min_shift_sat((*i).time);
                if(b<a){
                    a = b;
                }
            }
        }
        return a;
    }

    // TODO the following is a super conservative implementation - (how) can we do better ?
    double until_transducer::compute_lower_rob() {

        //	cout << "Getting into until_transducer::compute_lower_rob" << endl;
        double a,b;
        if (!get_param(I->begin_str,a)) a = I->begin;
        if (!get_param(I->end_str,b)) b = I->end;

        if (childL->compute_lower_rob()==BOTTOM) return BOTTOM;
        if (childR->compute_lower_rob()==BOTTOM) return BOTTOM;

        z_low.compute_timed_until(childL->z_low,childR->z_low, a, b);
        double et =min(z_up.endTime,end_time);
        z_low.resize(start_time,max(start_time,et),0.);

        if (z_low.empty())
            return BOTTOM;
        else
            return z_low.front().value;

    }

    double until_transducer::compute_upper_rob() {

        double a,b;
        if (!get_param(I->begin_str,a)) a = I->begin;
        if (!get_param(I->end_str,b)) b = I->end;

        if (childL->compute_upper_rob()==TOP) return TOP;
        if (childR->compute_upper_rob()==TOP) return TOP;

        z_up.compute_timed_until(childL->z_up,childR->z_up, a, b);
        double et =min(z_up.endTime-b,end_time);
        z_up.resize(start_time,max(start_time,et),0.);

        if (z_up.empty())
            return TOP;
        else
            return z_up.front().value;
    }

}
