#include <iostream>
#include <map>
#include <queue>
#include <utility>
#include <climits>
#include <functional>
#include <iomanip>
#include <stack>
#include <unordered_set>
#include <set>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <limits>
#include <unordered_map>
#include <cmath>

using namespace std;

#define SET(func_name,type,var_name,_var_name) void func_name(type _var_name) { var_name = _var_name ;} 
#define GET(func_name,type,var_name) type func_name() const { return var_name ;}

class header;
class payload;
class packet;
class node;
class event;
class link; // new

// for simplicity, we use a const int to simulate the delay
// if you want to simulate the more details, you should revise it to be a class
const unsigned int ONE_HOP_DELAY = 10;
const unsigned int BROADCAST_ID = UINT_MAX;

// BROADCAST_ID means that all neighbors are receivers; UINT_MAX is the maximum value of unsigned int

class header {
    public:
        virtual ~header() {}

        SET(setSrcID, unsigned int , srcID, _srcID);
        SET(setDstID, unsigned int , dstID, _dstID);
        SET(setPreID, unsigned int , preID, _preID);
        SET(setNexID, unsigned int , nexID, _nexID);
        GET(getSrcID, unsigned int , srcID);
        GET(getDstID, unsigned int , dstID);
        GET(getPreID, unsigned int , preID);
        GET(getNexID, unsigned int , nexID);
        
        SET(setVisa, unsigned int , Visa, _Visa);
        GET(getVisa, unsigned int , Visa);
        SET(setParent, unsigned int , Parent, _Parent);
        GET(getParent, unsigned int , Parent);
        SET(setOldparent, unsigned int , Oldparent, _Oldparent);
        GET(getOldparent, unsigned int , Oldparent);
        SET(setCounter, unsigned int , Counter, _Counter);
        GET(getCounter, unsigned int , Counter);
        SET(setCheatParent, unsigned int , CheatParent, _CheatParent);
        GET(getCheatParent, unsigned int , CheatParent);
        SET(setData, unsigned int , Data, _Data);
        GET(getData, unsigned int , Data);

        vector<int> new_children;

        

        virtual string type() = 0;
        
        // factory concept: generate a header
        class header_generator {
                // lock the copy constructor
                header_generator(header_generator &){}
                // store all possible types of header
                static map<string,header_generator*> prototypes;
            protected:
                // allow derived class to use it
                header_generator() {}
                // after you create a new header type, please register the factory of this header type by this function
                void register_header_type(header_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your header
                virtual header* generate() = 0 ;
            public:
                // you have to implement your own type() to return your header type
        	    virtual string type() = 0 ;
        	    // this function is used to generate any type of header derived
        	    static header * generate (string type) {
            		if(prototypes.find(type) != prototypes.end()){ // if this type derived exists 
            			return prototypes[type]->generate(); // generate it!!
            		}
            		std::cerr << "no such header type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered header types: " << endl;
            	    for (map<string,header::header_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~header_generator(){};
        };
        
    protected:
        header():srcID(BROADCAST_ID),dstID(BROADCAST_ID),preID(BROADCAST_ID),nexID(BROADCAST_ID),Data(0),Parent(BROADCAST_ID),Oldparent(BROADCAST_ID){} // this constructor cannot be directly called by users

    private:
        unsigned int srcID;
        unsigned int dstID;
        unsigned int preID;
        unsigned int nexID;
        unsigned int Visa;
        unsigned int Parent;
        unsigned int Oldparent;
        unsigned int Counter;
        unsigned int CheatParent;
        unsigned int Data;
        

        header(header &) {} // this constructor cannot be directly called by users
};
map<string,header::header_generator*> header::header_generator::prototypes;

class IoT_data_header : public header{
        IoT_data_header(IoT_data_header&){} // cannot be called by users
        
    protected:
        IoT_data_header(){} // this constructor cannot be directly called by users

    public:
        ~IoT_data_header(){}
        string type() { return "IoT_data_header"; }

        class IoT_data_header_generator;
        friend class IoT_data_header_generator;
        // IoT_data_header_generator is derived from header_generator to generate a header
        class IoT_data_header_generator : public header_generator{
                static IoT_data_header_generator sample;
                // this constructor is only for sample to register this header type
                IoT_data_header_generator() { /*cout << "IoT_data_header registered" << endl;*/ register_header_type(&sample); }
            protected:
                virtual header * generate(){
                    // cout << "IoT_data_header generated" << endl;
                    return new IoT_data_header; 
                }
            public:
                virtual string type() { return "IoT_data_header";}
                ~IoT_data_header_generator(){}
        
        };
};
IoT_data_header::IoT_data_header_generator IoT_data_header::IoT_data_header_generator::sample;

class IoT_ctrl_header : public header{
        IoT_ctrl_header(IoT_ctrl_header&){} // cannot be called by users
        
    protected:
        IoT_ctrl_header(){} // this constructor cannot be directly called by users

    public:
        ~IoT_ctrl_header(){}
        string type() { return "IoT_ctrl_header"; }

        class IoT_ctrl_header_generator;
        friend class IoT_ctrl_header_generator;
        // IoT_ctrl_header_generator is derived from header_generator to generate a header
        class IoT_ctrl_header_generator : public header_generator{
                static IoT_ctrl_header_generator sample;
                // this constructor is only for sample to register this header type
                IoT_ctrl_header_generator() { /*cout << "IoT_ctrl_header registered" << endl;*/ register_header_type(&sample); }
            protected:
                virtual header * generate(){
                    // cout << "IoT_ctrl_header generated" << endl;
                    return new IoT_ctrl_header; 
                }
            public:
                virtual string type() { return "IoT_ctrl_header";}
                ~IoT_ctrl_header_generator(){}
        
        };
};
IoT_ctrl_header::IoT_ctrl_header_generator IoT_ctrl_header::IoT_ctrl_header_generator::sample;


class AGG_ctrl_header : public header{
        AGG_ctrl_header(AGG_ctrl_header&){} // cannot be called by users
        
    protected:
        AGG_ctrl_header(){} // this constructor cannot be directly called by users

    public:
        ~AGG_ctrl_header(){}
        string type() { return "AGG_ctrl_header"; }

        class AGG_ctrl_header_generator;
        friend class AGG_ctrl_header_generator;
        // AGG_ctrl_header_generator is derived from header_generator to generate a header
        class AGG_ctrl_header_generator : public header_generator{
                static AGG_ctrl_header_generator sample;
                // this constructor is only for sample to register this header type
                AGG_ctrl_header_generator() { /*cout << "AGG_ctrl_header registered" << endl;*/ register_header_type(&sample); }
            protected:
                virtual header * generate(){
                    // cout << "AGG_ctrl_header generated" << endl;
                    return new AGG_ctrl_header; 
                }
            public:
                virtual string type() { return "AGG_ctrl_header";}
                ~AGG_ctrl_header_generator(){}
        
        };
};
AGG_ctrl_header::AGG_ctrl_header_generator AGG_ctrl_header::AGG_ctrl_header_generator::sample;


class DIS_ctrl_header : public header{
        DIS_ctrl_header(DIS_ctrl_header&){} // cannot be called by users
        
    protected:
        DIS_ctrl_header(){} // this constructor cannot be directly called by users

    public:
        ~DIS_ctrl_header(){}
        string type() { return "DIS_ctrl_header"; }

        class DIS_ctrl_header_generator;
        friend class DIS_ctrl_header_generator;
        // DIS_ctrl_header_generator is derived from header_generator to generate a header
        class DIS_ctrl_header_generator : public header_generator{
                static DIS_ctrl_header_generator sample;
                // this constructor is only for sample to register this header type
                DIS_ctrl_header_generator() { /*cout << "DIS_ctrl_header registered" << endl;*/ register_header_type(&sample); }
            protected:
                virtual header * generate(){
                    // cout << "DIS_ctrl_header generated" << endl;
                    return new DIS_ctrl_header; 
                }
            public:
                virtual string type() { return "DIS_ctrl_header";}
                ~DIS_ctrl_header_generator(){}
        
        };
};
DIS_ctrl_header::DIS_ctrl_header_generator DIS_ctrl_header::DIS_ctrl_header_generator::sample;

class payload {
        payload(payload&){} // this constructor cannot be directly called by users
        
        string msg;
        
    protected:
        payload(){}
    public:
        virtual ~payload(){}
        virtual string type() = 0;
        
        SET(setMsg,string,msg,_msg);
        GET(getMsg,string,msg);
        
        class payload_generator {
                // lock the copy constructor
                payload_generator(payload_generator &){}
                // store all possible types of header
                static map<string,payload_generator*> prototypes;
            protected:
                // allow derived class to use it
                payload_generator() {}
                // after you create a new payload type, please register the factory of this payload type by this function
                void register_payload_type(payload_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your payload
                virtual payload* generate() = 0;
            public:
                // you have to implement your own type() to return your header type
        	    virtual string type() = 0;
        	    // this function is used to generate any type of header derived
        	    static payload * generate (string type) {
            		if(prototypes.find(type) != prototypes.end()){ // if this type derived exists 
            			return prototypes[type]->generate(); // generate it!!
            		}
            		std::cerr << "no such payload type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered payload types: " << endl;
            	    for (map<string,payload::payload_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~payload_generator(){};
        };
};
map<string,payload::payload_generator*> payload::payload_generator::prototypes;


class IoT_data_payload : public payload {
        IoT_data_payload(IoT_data_payload&){}

    protected:
        IoT_data_payload(){} // this constructor cannot be directly called by users
    public:
        ~IoT_data_payload(){}
        
        string type() { return "IoT_data_payload"; }
        
        class IoT_data_payload_generator;
        friend class IoT_data_payload_generator;
        // IoT_data_payload is derived from payload_generator to generate a payload
        class IoT_data_payload_generator : public payload_generator{
                static IoT_data_payload_generator sample;
                // this constructor is only for sample to register this payload type
                IoT_data_payload_generator() { /*cout << "IoT_data_payload registered" << endl;*/ register_payload_type(&sample); }
            protected:
                virtual payload * generate(){ 
                    // cout << "IoT_data_payload generated" << endl;
                    return new IoT_data_payload; 
                }
            public:
                virtual string type() { return "IoT_data_payload";}
                ~IoT_data_payload_generator(){}
        };
};
IoT_data_payload::IoT_data_payload_generator IoT_data_payload::IoT_data_payload_generator::sample;

class IoT_ctrl_payload : public payload {
        IoT_ctrl_payload(IoT_ctrl_payload & s): counter (s.counter) {}
        
        unsigned int counter ;
        
    protected:
        IoT_ctrl_payload(): counter (0) {} // this constructor cannot be directly called by users
    public:
        ~IoT_ctrl_payload(){}
        
        void increase() { counter ++; } // used to increase the counter
        GET(getCounter,unsigned int,counter); // used to get the value of counter
        
        string type() { return "IoT_ctrl_payload"; }
        
        
        class IoT_ctrl_payload_generator;
        friend class IoT_ctrl_payload_generator;
        // IoT_data_payload is derived from payload_generator to generate a payload
        class IoT_ctrl_payload_generator : public payload_generator{
                static IoT_ctrl_payload_generator sample;
                // this constructor is only for sample to register this payload type
                IoT_ctrl_payload_generator() { /*cout << "IoT_ctrl_payload registered" << endl;*/ register_payload_type(&sample); }
            protected:
                virtual payload * generate(){ 
                    // cout << "IoT_ctrl_payload generated" << endl;
                    return new IoT_ctrl_payload; 
                }
            public:
                virtual string type() { return "IoT_ctrl_payload";}
                ~IoT_ctrl_payload_generator(){}
        };
};
IoT_ctrl_payload::IoT_ctrl_payload_generator IoT_ctrl_payload::IoT_ctrl_payload_generator::sample;


class AGG_ctrl_payload : public payload {
        AGG_ctrl_payload(AGG_ctrl_payload & s) {}
        
        // unsigned int counter ;
        
    protected:
        AGG_ctrl_payload() {} // this constructor cannot be directly called by users
    public:
        ~AGG_ctrl_payload(){}
        
        // void increase() { counter ++; } // used to increase the counter
        // GET(getCounter,unsigned int,counter); // used to get the value of counter
        
        string type() { return "AGG_ctrl_payload"; }
        
        
        class AGG_ctrl_payload_generator;
        friend class AGG_ctrl_payload_generator;
        // IoT_data_payload is derived from payload_generator to generate a payload
        class AGG_ctrl_payload_generator : public payload_generator{
                static AGG_ctrl_payload_generator sample;
                // this constructor is only for sample to register this payload type
                AGG_ctrl_payload_generator() { /*cout << "AGG_ctrl_payload registered" << endl;*/ register_payload_type(&sample); }
            protected:
                virtual payload * generate(){ 
                    // cout << "AGG_ctrl_payload generated" << endl;
                    return new AGG_ctrl_payload; 
                }
            public:
                virtual string type() { return "AGG_ctrl_payload";}
                ~AGG_ctrl_payload_generator(){}
        };
};
AGG_ctrl_payload::AGG_ctrl_payload_generator AGG_ctrl_payload::AGG_ctrl_payload_generator::sample;


class DIS_ctrl_payload : public payload {
        DIS_ctrl_payload(DIS_ctrl_payload & s): parent(s.parent) {}
        
        // unsigned int counter ;
        unsigned int parent;
        
    protected:
        DIS_ctrl_payload(int _parent=0): parent (_parent) {} // this constructor cannot be directly called by users
    public:
        ~DIS_ctrl_payload(){}
        
        // void increase() { counter ++; } // used to increase the counter
        SET(setParent,unsigned int,parent,_parent);
        GET(getParent,unsigned int,parent); // used to get the value of counter
        
        string type() { return "DIS_ctrl_payload"; }
        
        
        class DIS_ctrl_payload_generator;
        friend class DIS_ctrl_payload_generator;
        // IoT_data_payload is derived from payload_generator to generate a payload
        class DIS_ctrl_payload_generator : public payload_generator{
                static DIS_ctrl_payload_generator sample;
                // this constructor is only for sample to register this payload type
                DIS_ctrl_payload_generator() { /*cout << "DIS_ctrl_payload registered" << endl;*/ register_payload_type(&sample); }
            protected:
                virtual payload * generate(){ 
                    // cout << "DIS_ctrl_payload generated" << endl;
                    return new DIS_ctrl_payload; 
                }
            public:
                virtual string type() { return "DIS_ctrl_payload";}
                ~DIS_ctrl_payload_generator(){}
        };
};
DIS_ctrl_payload::DIS_ctrl_payload_generator DIS_ctrl_payload::DIS_ctrl_payload_generator::sample;


class packet{
        // a packet usually contains a header and a payload
        header *hdr;
        payload *pld;
        unsigned int p_id;
        static unsigned int last_packet_id ; 
        
        packet(packet &) {}
        static int live_packet_num ;
    protected:
        // these constructors cannot be directly called by users
        packet(): hdr(nullptr), pld(nullptr) { p_id=last_packet_id++; live_packet_num ++; }
        packet(string _hdr, string _pld, bool rep = false, unsigned int rep_id = 0) {
            if (! rep ) // a duplicated packet does not have a new packet id
                p_id = last_packet_id ++;
            else
                p_id = rep_id;
            hdr = header::header_generator::generate(_hdr); 
            pld = payload::payload_generator::generate(_pld); 
            live_packet_num ++;
        }
    public:
        virtual ~packet(){ 
            // cout << "packet destructor begin" << endl;
            if (hdr != nullptr) 
                delete hdr; 
            if (pld != nullptr) 
                delete pld; 
            live_packet_num --;
            // cout << "packet destructor end" << endl;
        }
        
        SET(setHeader,header*,hdr,_hdr);
        GET(getHeader,header*,hdr);
        SET(setPayload,payload*,pld,_pld);
        GET(getPayload,payload*,pld);
        GET(getPacketID,unsigned int,p_id);
        
        static void discard ( packet* &p ) {
            // cout << "checking" << endl;
            if (p != nullptr) {
                // cout << "discarding" << endl;
                // cout << p->type() << endl;
                delete p;
                // cout << "discarded" << endl;
            }
            p = nullptr;
            // cout << "checked" << endl;
        }
        virtual string type () = 0;
        // you can define your own packet's addition_information
        // to print more information for recv_event and send_event
        virtual string addition_information () { return ""; }
        
        static int getLivePacketNum () { return live_packet_num; }
        
        class packet_generator;
        friend class packet_generator;
        class packet_generator {
                // lock the copy constructor
                packet_generator(packet_generator &){}
                // store all possible types of packet
                static map<string,packet_generator*> prototypes;
            protected:
                // allow derived class to use it
                packet_generator() {}
                // after you create a new packet type, please register the factory of this payload type by this function
                void register_packet_type(packet_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your payload
                virtual packet* generate ( packet *p = nullptr) = 0;
            public:
                // you have to implement your own type() to return your packet type
        	    virtual string type() = 0;
        	    // this function is used to generate any type of packet derived
        	    static packet * generate (string type) {
            		if(prototypes.find(type) != prototypes.end()){ // if this type derived exists 
            			return prototypes[type]->generate(); // generate it!!
            		}
            		std::cerr << "no such packet type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static packet * replicate (packet *p) {
            	    if(prototypes.find(p->type()) != prototypes.end()){ // if this type derived exists 
            			return prototypes[p->type()]->generate(p); // generate it!!
            		}
            		std::cerr << "no such packet type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered packet types: " << endl;
            	    for (map<string,packet::packet_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~packet_generator(){};
        };
};
map<string,packet::packet_generator*> packet::packet_generator::prototypes;
unsigned int packet::last_packet_id = 0 ;
int packet::live_packet_num = 0;


// this packet is used to transmit the data
class IoT_data_packet: public packet {
        IoT_data_packet(IoT_data_packet &) {}
        
    protected:
        IoT_data_packet(){} // this constructor cannot be directly called by users
        IoT_data_packet(packet*p): packet(p->getHeader()->type(), p->getPayload()->type(), true, p->getPacketID()) {
            *(dynamic_cast<IoT_data_header*>(this->getHeader())) = *(dynamic_cast<IoT_data_header*> (p->getHeader()));
            *(dynamic_cast<IoT_data_payload*>(this->getPayload())) = *(dynamic_cast<IoT_data_payload*> (p->getPayload()));
            //DFS_path = (dynamic_cast<IoT_data_header*>(p))->DFS_path;
            //isVisited = (dynamic_cast<IoT_data_header*>(p))->isVisited;
        } // for duplicate
        IoT_data_packet(string _h, string _p): packet(_h,_p) {}
        
    public:
        virtual ~IoT_data_packet(){}
        string type() { return "IoT_data_packet"; }
        
        class IoT_data_packet_generator;
        friend class IoT_data_packet_generator;
        // IoT_data_packet is derived from packet_generator to generate a pub packet
        class IoT_data_packet_generator : public packet_generator{
                static IoT_data_packet_generator sample;
                // this constructor is only for sample to register this packet type
                IoT_data_packet_generator() { /*cout << "IoT_data_packet registered" << endl;*/ register_packet_type(&sample); }
            protected:
                virtual packet *generate (packet *p = nullptr){
                    // cout << "IoT_data_packet generated" << endl;
                    if ( nullptr == p )
                        return new IoT_data_packet("IoT_data_header","IoT_data_payload"); 
                    else
                        return new IoT_data_packet(p); // duplicate
                }
            public:
                virtual string type() { return "IoT_data_packet";}
                ~IoT_data_packet_generator(){}
        };
};
IoT_data_packet::IoT_data_packet_generator IoT_data_packet::IoT_data_packet_generator::sample;

// this packet type is used to conduct distributed BFS
class IoT_ctrl_packet: public packet {
        IoT_ctrl_packet(IoT_ctrl_packet &) {}
        
    protected:
        IoT_ctrl_packet(){} // this constructor cannot be directly called by users
        IoT_ctrl_packet(packet*p): packet(p->getHeader()->type(), p->getPayload()->type(), true, p->getPacketID()) {
            *(dynamic_cast<IoT_ctrl_header*>(this->getHeader())) = *(dynamic_cast<IoT_ctrl_header*> (p->getHeader()));
            *(dynamic_cast<IoT_ctrl_payload*>(this->getPayload())) = *(dynamic_cast<IoT_ctrl_payload*> (p->getPayload()));
            //DFS_path = (dynamic_cast<IoT_ctrl_header*>(p))->DFS_path;
            //isVisited = (dynamic_cast<IoT_ctrl_header*>(p))->isVisited;
        } // for duplicate
        IoT_ctrl_packet(string _h, string _p): packet(_h,_p) {}
        
    public:
        virtual ~IoT_ctrl_packet(){}
        string type() { return "IoT_ctrl_packet"; }
        virtual string addition_information() {
            unsigned int counter = (dynamic_cast<IoT_ctrl_payload*>(this->getPayload()))->getCounter();
            // cout << counter << endl;
            return " counter " + to_string(counter);
        }
        
        class IoT_ctrl_packet_generator;
        friend class IoT_ctrl_packet_generator;
        // IoT_ctrl_packet is derived from packet_generator to generate a pub packet
        class IoT_ctrl_packet_generator : public packet_generator{
                static IoT_ctrl_packet_generator sample;
                // this constructor is only for sample to register this packet type
                IoT_ctrl_packet_generator() { /*cout << "IoT_ctrl_packet registered" << endl;*/ register_packet_type(&sample); }
            protected:
                virtual packet *generate (packet *p = nullptr){
                    // cout << "IoT_ctrl_packet generated" << endl;
                    if ( nullptr == p )
                        return new IoT_ctrl_packet("IoT_ctrl_header","IoT_ctrl_payload"); 
                    else
                        return new IoT_ctrl_packet(p); // duplicate
                }
            public:
                virtual string type() { return "IoT_ctrl_packet";}
                ~IoT_ctrl_packet_generator(){}
        };
};
IoT_ctrl_packet::IoT_ctrl_packet_generator IoT_ctrl_packet::IoT_ctrl_packet_generator::sample;



// this packet type is used to transmit each device's nblist to the sink
class AGG_ctrl_packet: public packet {
        AGG_ctrl_packet(AGG_ctrl_packet &) {}
        
    protected:
        AGG_ctrl_packet(){} // this constructor cannot be directly called by users
        AGG_ctrl_packet(packet*p): packet(p->getHeader()->type(), p->getPayload()->type(), true, p->getPacketID()) {
            *(dynamic_cast<AGG_ctrl_header*>(this->getHeader())) = *(dynamic_cast<AGG_ctrl_header*> (p->getHeader()));
            *(dynamic_cast<AGG_ctrl_payload*>(this->getPayload())) = *(dynamic_cast<AGG_ctrl_payload*> (p->getPayload()));
            //DFS_path = (dynamic_cast<AGG_ctrl_header*>(p))->DFS_path;
            //isVisited = (dynamic_cast<AGG_ctrl_header*>(p))->isVisited;
        } // for duplicate
        AGG_ctrl_packet(string _h, string _p): packet(_h,_p) {}
        
    public:
        virtual ~AGG_ctrl_packet(){}
        string type() { return "AGG_ctrl_packet"; }
        
        // virtual string addition_information() {
        //     string msg = (dynamic_cast<AGG_ctrl_payload*>(this->getPayload()))->getMsg();
        //     return " msg " + msg;
        // }
        
        class AGG_ctrl_packet_generator;
        friend class AGG_ctrl_packet_generator;
        // AGG_ctrl_packet is derived from packet_generator to generate a pub packet
        class AGG_ctrl_packet_generator : public packet_generator{
                static AGG_ctrl_packet_generator sample;
                // this constructor is only for sample to register this packet type
                AGG_ctrl_packet_generator() { /*cout << "AGG_ctrl_packet registered" << endl;*/ register_packet_type(&sample); }
            protected:
                virtual packet *generate (packet *p = nullptr){
                    // cout << "AGG_ctrl_packet generated" << endl;
                    if ( nullptr == p )
                        return new AGG_ctrl_packet("AGG_ctrl_header","AGG_ctrl_payload"); 
                    else
                        return new AGG_ctrl_packet(p); // duplicate
                }
            public:
                virtual string type() { return "AGG_ctrl_packet";}
                ~AGG_ctrl_packet_generator(){}
        };
};
AGG_ctrl_packet::AGG_ctrl_packet_generator AGG_ctrl_packet::AGG_ctrl_packet_generator::sample;


// this packet type is used to transmit the new parent to each device
class DIS_ctrl_packet: public packet {
        DIS_ctrl_packet(DIS_ctrl_packet &) {}
        
    protected:
        DIS_ctrl_packet(){} // this constructor cannot be directly called by users
        DIS_ctrl_packet(packet*p): packet(p->getHeader()->type(), p->getPayload()->type(), true, p->getPacketID()) {
            *(dynamic_cast<DIS_ctrl_header*>(this->getHeader())) = *(dynamic_cast<DIS_ctrl_header*> (p->getHeader()));
            *(dynamic_cast<DIS_ctrl_payload*>(this->getPayload())) = *(dynamic_cast<DIS_ctrl_payload*> (p->getPayload()));
            //DFS_path = (dynamic_cast<DIS_ctrl_header*>(p))->DFS_path;
            //isVisited = (dynamic_cast<DIS_ctrl_header*>(p))->isVisited;
        } // for duplicate
        DIS_ctrl_packet(string _h, string _p): packet(_h,_p) {}
        
    public:
        virtual ~DIS_ctrl_packet(){}
        string type() { return "DIS_ctrl_packet"; }
        virtual string addition_information() {
            unsigned int parent = (dynamic_cast<DIS_ctrl_payload*>(this->getPayload()))->getParent();
            // cout << counter << endl;
            return " parent " + to_string(parent);
        }
        
        class DIS_ctrl_packet_generator;
        friend class DIS_ctrl_packet_generator;
        // DIS_ctrl_packet is derived from packet_generator to generate a pub packet
        class DIS_ctrl_packet_generator : public packet_generator{
                static DIS_ctrl_packet_generator sample;
                // this constructor is only for sample to register this packet type
                DIS_ctrl_packet_generator() { /*cout << "DIS_ctrl_packet registered" << endl;*/ register_packet_type(&sample); }
            protected:
                virtual packet *generate (packet *p = nullptr){
                    // cout << "DIS_ctrl_packet generated" << endl;
                    if ( nullptr == p )
                        return new DIS_ctrl_packet("DIS_ctrl_header","DIS_ctrl_payload"); 
                    else
                        return new DIS_ctrl_packet(p); // duplicate
                }
            public:
                virtual string type() { return "DIS_ctrl_packet";}
                ~DIS_ctrl_packet_generator(){}
        };
};
DIS_ctrl_packet::DIS_ctrl_packet_generator DIS_ctrl_packet::DIS_ctrl_packet_generator::sample;




class node {
        // all nodes created in the program
        static map<unsigned int, node*> id_node_table;
        
        unsigned int id;
        map<unsigned int,bool> phy_neighbors;
        unsigned int size;
        int packet_size;

    protected:
        node(node&){} // this constructor should not be used
        node(){} // this constructor should not be used
        node(unsigned int _id): id(_id) { id_node_table[_id] = this; }
    public:
        virtual ~node() { // erase the node
            id_node_table.erase (id) ;
        }
        virtual string type() = 0; // please define it in your derived node class
        
        void add_phy_neighbor (unsigned int _id, string link_type = "simple_link"); // we only add a directed link from id to _id
        void del_phy_neighbor (unsigned int _id); // we only delete a directed link from id to _id
        
        void add_data_size(unsigned int s){
            size = s;
        };

        unsigned int get_data_size(){
            return size;
        }

        void set_packet_size(int i){
            packet_size=i;
        }

        int get_packet_size(){
            return packet_size;
        }

        // you can use the function to get the node's neigbhors at this time
        // but in the project 3, you are not allowed to use this function 
        const map<unsigned int,bool> & getPhyNeighbors () { 
            return phy_neighbors;
        }
        
        

        void recv (packet *p) {
            packet *tp = p;
            recv_handler(tp); 
            packet::discard(p); 
        } // the packet will be directly deleted after the handler
        void send (packet *p);
        
        // receive the packet and do something; this is a pure virtual function
        virtual void recv_handler(packet *p) = 0;
        void send_handler(packet *P);
        
        static node * id_to_node (unsigned int _id) { return ((id_node_table.find(_id)!=id_node_table.end()) ? id_node_table[_id]: nullptr) ; }
        GET(getNodeID,unsigned int,id);
        
        static void del_node (unsigned int _id) {
            if (id_node_table.find(_id) != id_node_table.end())
                id_node_table.erase(_id);
        }
        static unsigned int getNodeNum () { return id_node_table.size(); }

        class node_generator {
                // lock the copy constructor
                node_generator(node_generator &){}
                // store all possible types of node
                static map<string,node_generator*> prototypes;
            protected:
                // allow derived class to use it
                node_generator() {}
                // after you create a new node type, please register the factory of this node type by this function
                void register_node_type(node_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your node
                virtual node* generate(unsigned int _id) = 0;
            public:
                // you have to implement your own type() to return your node type
        	    virtual string type() = 0;
        	    // this function is used to generate any type of node derived
        	    static node * generate (string type, unsigned int _id) {
        	        if(id_node_table.find(_id)!=id_node_table.end()){
        	            std::cerr << "duplicate node id" << std::endl; // node id is duplicated
        	            return nullptr;
        	        }
        	        else if ( BROADCAST_ID == _id ) {
        	            cerr << "BROADCAST_ID cannot be used" << endl;
        	            return nullptr;
        	        }
            		else if(prototypes.find(type) != prototypes.end()){ // if this type derived exists 
            		    node * created_node = prototypes[type]->generate(_id);
            			return created_node; // generate it!!
            		}
            		std::cerr << "no such node type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered node types: " << endl;
            	    for (map<string,node::node_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~node_generator(){};
        };
};
map<string,node::node_generator*> node::node_generator::prototypes;
map<unsigned int,node*> node::id_node_table;

class IoT_device: public node {
        // map<unsigned int,bool> one_hop_neighbors; // you can use this variable to record the node's 1-hop neighbors
        bool hi;

        unsigned parent=99999;
        std::vector<unsigned int> children;
        std::vector<unsigned int> phyconnect;

    protected:
        IoT_device() {} // it should not be used
        IoT_device(IoT_device&) {} // it should not be used
        IoT_device(unsigned int _id): node(_id), hi(false) {} // this constructor cannot be directly called by users
    
    public:
        unsigned int GetParent() { return parent; }
        void SetParent(unsigned int p) { parent = p; }

        void AddChild(unsigned int c) { children.push_back(c); }
        void DeleteAllChild() { children.clear(); }
        void AddConnect(unsigned int c) { phyconnect.push_back(c); }
        void DeleConnect() { phyconnect.pop_back(); }
        vector<unsigned int> GetPhyConnect() { return phyconnect;}
        void DeleChild(unsigned int c) { 
            auto it = std::remove(children.begin(), children.end(), c);
            if (it != children.end()) {
                // Use erase to remove the elements from the vector
                children.erase(it, children.end());
            }
            
        
        }
        bool FindChild(unsigned int c) {  
            vector<unsigned int>::iterator it = std::find(children.begin(), children.end(), c);
            if(it!=children.end()){
                return true;
            }
            else{
                return false;
            }
        }
        void DisplayChildren(){
            if(children.size()==0){
                cout << "no child\n";
            }
            for (size_t i = 0; i < children.size(); ++i) {
                if (i != 0) {
                    cout<< ", ";
                }
                cout << children[i];
            }
            cout << "\n";
        }

            bool IsThereChild(){
            if(children.size()==0){
                return false;
            }
            else{
                return true;
            }

        }

        bool FindConnect(unsigned int c) {  
            vector<unsigned int>::iterator it = std::find(phyconnect.begin(),phyconnect.end(), c);
            if(it!=phyconnect.end()){
                return true;
            }
            else{
                return false;
            }
        }


        ~IoT_device(){}
        string type() { return "IoT_device"; }
        
        // please define recv_handler function to deal with the incoming packet
        virtual void recv_handler (packet *p);
        
        // void add_one_hop_neighbor (unsigned int n_id) { one_hop_neighbors[n_id] = true; }
        // unsigned int get_one_hop_neighbor_num () { return one_hop_neighbors.size(); }
        
        class IoT_device_generator;
        friend class IoT_device_generator;
        // IoT_device is derived from node_generator to generate a node
        class IoT_device_generator : public node_generator{
                static IoT_device_generator sample;
                // this constructor is only for sample to register this node type
                IoT_device_generator() { /*cout << "IoT_device registered" << endl;*/ register_node_type(&sample); }
            protected:
                virtual node * generate(unsigned int _id){ /*cout << "IoT_device generated" << endl;*/ return new IoT_device(_id); }
            public:
                virtual string type() { return "IoT_device";}
                ~IoT_device_generator(){}
        };

        void initialize_surround_map(int Nodes){
            for (int key = 0; key <= Nodes; ++key) {
                Smallest_Parent_Id[key] = 99999;
                Smallest_Counter_Id[key] = 99999;
            }
        }

        int flag = 1;
        unsigned  int counter=99999;
        int id = 0;
        map<unsigned int,unsigned int> Smallest_Parent_Id;
        map<unsigned int,unsigned int> Smallest_Counter_Id;
        map<unsigned int,unsigned int> Reversed_Path_Map;
        string AggMsg="default";
};
IoT_device::IoT_device_generator IoT_device::IoT_device_generator::sample;

class IoT_sink: public node {
        // map<unsigned int,bool> one_hop_neighbors; // you can use this variable to record the node's 1-hop neighbors 
        
        bool hi; // this is used for example; you can remove it when doing hw2

        unsigned int parent;
        std::vector<unsigned int> children;

    protected:
        IoT_sink() {} // it should not be used
        IoT_sink(IoT_sink&) {} // it should not be used
        IoT_sink(unsigned int _id): node(_id), hi(false) {} // this constructor cannot be directly called by users
    
    public:
        ~IoT_sink(){}
        string type() { return "IoT_sink"; }
        
        // please define recv_handler function to deal with the incoming packet
        virtual void recv_handler (packet *p);
        
        // void add_one_hop_neighbor (unsigned int n_id) { one_hop_neighbors[n_id] = true; }
        // unsigned int get_one_hop_neighbor_num () { return one_hop_neighbors.size(); }
        
        class IoT_sink_generator;
        friend class IoT_sink_generator;
        // IoT_sink is derived from node_generator to generate a node
        class IoT_sink_generator : public node_generator{
                static IoT_sink_generator sample;
                // this constructor is only for sample to register this node type
                IoT_sink_generator() { /*cout << "IoT_sink registered" << endl;*/ register_node_type(&sample); }
            protected:
                virtual node * generate(unsigned int _id){ /*cout << "IoT_sink generated" << endl;*/ return new IoT_sink(_id); }
            public:
                virtual string type() { return "IoT_sink";}
                ~IoT_sink_generator(){}
        };
        map<unsigned int,unsigned int> Reversed_Path_Map;
        map<unsigned int, vector<unsigned int> > graph;
        map<unsigned int,unsigned int> new_parent;
        map<unsigned int,unsigned int> level;
        map<unsigned int,unsigned int> data;
       
        void printGraph() const {
            for (const auto& pair : graph) {
                std::cout << "Node " << pair.first << ": ";
                for (const auto& neighbor : pair.second) {
                    std::cout << neighbor << " ";
                }
                std::cout << std::endl;
            }
        };
    
};
IoT_sink::IoT_sink_generator IoT_sink::IoT_sink_generator::sample;

class mycomp {
    bool reverse;
    
    public:
        mycomp(const bool& revparam = false) { reverse=revparam ; }
        bool operator() (const event* lhs, const event* rhs) const;
};

class event {
        event(event*&){} // this constructor cannot be directly called by users
        static priority_queue < event*, vector < event* >, mycomp > events;
        static unsigned int cur_time; // timer
        static unsigned int end_time;
        
        // get the next event
        static event * get_next_event() ;
        static void add_event (event *e) { events.push(e); }
        static hash<string> event_seq;
        
    protected:
        unsigned int trigger_time;
        
        event(){} // it should not be used
        event(unsigned int _trigger_time): trigger_time(_trigger_time) {}

    public:
        virtual void trigger()=0;
        virtual ~event(){}

        virtual unsigned int event_priority() const = 0;
        unsigned int get_hash_value(string string_for_hash) const {
            unsigned int priority = event_seq (string_for_hash);
            return priority;
        }
        
        static void flush_events (); // only for debug
        
        GET(getTriggerTime,unsigned int,trigger_time);
        
        static void start_simulate( unsigned int _end_time ); // the function is used to start the simulation
        
        static unsigned int getCurTime() { return cur_time ; }
        static void getCurTime(unsigned int _cur_time) { cur_time = _cur_time; } 
        // static unsigned int getEndTime() { return end_time ; }
        // static void getEndTime(unsigned int _end_time) { end_time = _end_time; }
        
        virtual void print () const = 0; // the function is used to print the event information

        class event_generator{
                // lock the copy constructor
                event_generator(event_generator &){}
                // store all possible types of event
                static map<string,event_generator*> prototypes;
            protected:
                // allow derived class to use it
                event_generator() {}
                // after you create a new event type, please register the factory of this event type by this function
                void register_event_type(event_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your event
                virtual event* generate(unsigned int _trigger_time, void * data) = 0;
            public:
                // you have to implement your own type() to return your event type
        	    virtual string type() = 0;
        	    // this function is used to generate any type of event derived
        	    static event * generate (string type, unsigned int _trigger_time, void * data) {
            		if(prototypes.find(type) != prototypes.end()){ // if this type derived exists
            		    event * e = prototypes[type]->generate(_trigger_time, data);
            		    add_event(e);
            		    return e; // generate it!!
            		}
            		std::cerr << "no such event type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered event types: " << endl;
            	    for (map<string,event::event_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~event_generator(){}
        };
};
map<string,event::event_generator*> event::event_generator::prototypes;
priority_queue < event*, vector< event* >, mycomp > event::events;
hash<string> event::event_seq;

unsigned int event::cur_time = 0;
unsigned int event::end_time = 0;

void event::flush_events()
{ 
    cout << "**flush begin" << endl;
    while ( ! events.empty() ) {
        cout << setw(11) << events.top()->trigger_time << ": " << setw(11) << events.top()->event_priority() << endl;
        delete events.top();
        events.pop();
    }
    cout << "**flush end" << endl;
}
event * event::get_next_event() {
    if(events.empty()) 
        return nullptr; 
    event * e = events.top();
    events.pop(); 
    // cout << events.size() << " events remains" << endl;
    return e; 
}
void event::start_simulate(unsigned int _end_time) {
    if (_end_time<0) {
        cerr << "you should give a possitive value of _end_time" << endl;
        return;
    }
    end_time = _end_time;
    event *e; 
    e = event::get_next_event ();
    while ( e != nullptr && e->trigger_time <= end_time ) {
        if ( cur_time <= e->trigger_time )
            cur_time = e->trigger_time;
        else {
            cerr << "cur_time = " << cur_time << ", event trigger_time = " << e->trigger_time << endl;
            break;
        }

        // cout << "event trigger_time = " << e->trigger_time << endl;
        e->print(); // for log
        // cout << " event begin" << endl;
        e->trigger();
        // cout << " event end" << endl;
        delete e;
        e = event::get_next_event ();
    }
    // cout << "no more event" << endl;
}

bool mycomp::operator() (const event* lhs, const event* rhs) const {
    // cout << lhs->getTriggerTime() << ", " << rhs->getTriggerTime() << endl;
    // cout << lhs->type() << ", " << rhs->type() << endl;
    unsigned int lhs_pri = lhs->event_priority();
    unsigned int rhs_pri = rhs->event_priority();
    // cout << "lhs hash = " << lhs_pri << endl;
    // cout << "rhs hash = " << rhs_pri << endl;
    
    if (reverse) 
        return ((lhs->getTriggerTime()) == (rhs->getTriggerTime())) ? (lhs_pri < rhs_pri): ((lhs->getTriggerTime()) < (rhs->getTriggerTime()));
    else 
        return ((lhs->getTriggerTime()) == (rhs->getTriggerTime())) ? (lhs_pri > rhs_pri): ((lhs->getTriggerTime()) > (rhs->getTriggerTime()));
}

class recv_event: public event {
    public:
        class recv_data; // forward declaration
            
    private:
        recv_event(recv_event&) {} // this constructor cannot be used
        recv_event() {} // we don't allow users to new a recv_event by themselv
        unsigned int senderID; // the sender
        unsigned int receiverID; // the receiver; the packet will be given to the receiver
        packet *pkt; // the packet
        
    protected:
        // this constructor cannot be directly called by users; only by generator
        recv_event(unsigned int _trigger_time, void *data): event(_trigger_time), senderID(BROADCAST_ID), receiverID(BROADCAST_ID), pkt(nullptr){
            recv_data * data_ptr = (recv_data*) data;
            senderID = data_ptr->s_id;
            receiverID = data_ptr->r_id; // the packet will be given to the receiver
            pkt = data_ptr->_pkt;
        } 
        
    public:
        virtual ~recv_event(){}
        // recv_event will trigger the recv function
        virtual void trigger();
        
        unsigned int event_priority() const;
        
        class recv_event_generator;
        friend class recv_event_generator;
        // recv_event is derived from event_generator to generate a event
        class recv_event_generator : public event_generator{
                static recv_event_generator sample;
                // this constructor is only for sample to register this event type
                recv_event_generator() { /*cout << "recv_event registered" << endl;*/ register_event_type(&sample); }
            protected:
                virtual event * generate(unsigned int _trigger_time, void *data){ 
                    // cout << "recv_event generated" << endl; 
                    return new recv_event(_trigger_time, data); 
                }
                
            public:
                virtual string type() { return "recv_event";}
                ~recv_event_generator(){}
        };
        // this class is used to initialize the recv_event
        class recv_data{
            public:
                unsigned int s_id;
                unsigned int r_id;
                packet *_pkt;
        };
        
        void print () const;
};
recv_event::recv_event_generator recv_event::recv_event_generator::sample;

void recv_event::trigger() {
    if (pkt == nullptr) {
        cerr << "recv_event error: no pkt!" << endl; 
        return ; 
    }
    else if (node::id_to_node(receiverID) == nullptr){
        cerr << "recv_event error: no node " << receiverID << "!" << endl;
        delete pkt; return ;
    }
    node::id_to_node(receiverID)->recv(pkt); 
}
unsigned int recv_event::event_priority() const {
    string string_for_hash;
    string_for_hash = to_string(getTriggerTime()) + to_string(senderID) + to_string (receiverID) + to_string (pkt->getPacketID());
    return get_hash_value(string_for_hash);
}
// the recv_event::print() function is used for log file
void recv_event::print () const {
    cout << "time "          << setw(11) << event::getCurTime() 
         << "   recID"       << setw(11) << receiverID 
         << "   pktID"       << setw(11) << pkt->getPacketID()
         << "   srcID"       << setw(11) << pkt->getHeader()->getSrcID() 
         << "   dstID"       << setw(11) << pkt->getHeader()->getDstID() 
         << "   preID"       << setw(11) << pkt->getHeader()->getPreID()
         << "   nexID"       << setw(11) << pkt->getHeader()->getNexID()
         << "   "            << pkt->type()
         << pkt->addition_information();
//  if ( pkt->type() == "IoT_ctrl_packet" ) cout << "   " << ((IoT_ctrl_payload*)pkt->getPayload())->getCounter();
         cout << endl;
    // cout << pkt->type()
    //      << "   time "       << setw(11) << event::getCurTime() 
    //      << "   recID "      << setw(11) << receiverID 
    //      << "   pktID"       << setw(11) << pkt->getPacketID()
    //      << "   srcID "      << setw(11) << pkt->getHeader()->getSrcID() 
    //      << "   dstID"       << setw(11) << pkt->getHeader()->getDstID() 
    //      << "   preID"       << setw(11) << pkt->getHeader()->getPreID()
    //      << "   nexID"       << setw(11) << pkt->getHeader()->getNexID()
    //      << endl;
}

class send_event: public event {
    public:
        class send_data; // forward declaration
            
    private:
        send_event (send_event &){}
        send_event (){} // we don't allow users to new a recv_event by themselves
        // this constructor cannot be directly called by users; only by generator
        unsigned int senderID; // the sender
        unsigned int receiverID; // the receiver 
        packet *pkt; // the packet
    
    protected:
        send_event (unsigned int _trigger_time, void *data): event(_trigger_time), senderID(BROADCAST_ID), receiverID(BROADCAST_ID), pkt(nullptr){
            send_data * data_ptr = (send_data*) data;
            senderID = data_ptr->s_id;
            receiverID = data_ptr->r_id;
            pkt = data_ptr->_pkt;
        } 
        
    public:
        virtual ~send_event(){}
        // send_event will trigger the send function
        virtual void trigger();
        
        unsigned int event_priority() const;
        
        class send_event_generator;
        friend class send_event_generator;
        // send_event is derived from event_generator to generate a event
        class send_event_generator : public event_generator{
                static send_event_generator sample;
                // this constructor is only for sample to register this event type
                send_event_generator() { /*cout << "send_event registered" << endl;*/ register_event_type(&sample); }
            protected:
                virtual event * generate(unsigned int _trigger_time, void *data){ 
                    // cout << "send_event generated" << endl; 
                    return new send_event(_trigger_time, data); 
                }
            
            public:
                virtual string type() { return "send_event";}
                ~send_event_generator(){}
        };
        // this class is used to initialize the send_event
        class send_data{
            public:
                unsigned int s_id;
                unsigned int r_id;
                packet *_pkt;
                unsigned int t;
        };
        
        void print () const;
};
send_event::send_event_generator send_event::send_event_generator::sample;

void send_event::trigger() {
    if (pkt == nullptr) {
        cerr << "send_event error: no pkt!" << endl; 
        return ; 
    }
    else if (node::id_to_node(senderID) == nullptr){
        cerr << "send_event error: no node " << senderID << "!" << endl;
        delete pkt; return ;
    }
    node::id_to_node(senderID)->send(pkt);
}
unsigned int send_event::event_priority() const {
    string string_for_hash;
    string_for_hash = to_string(getTriggerTime()) + to_string(senderID) + to_string (receiverID) + to_string (pkt->getPacketID());
    return get_hash_value(string_for_hash);
}
// the send_event::print() function is used for log file
void send_event::print () const {
    cout << "time "          << setw(11) << event::getCurTime() 
         << "   senID"       << setw(11) << senderID 
         << "   pktID"       << setw(11) << pkt->getPacketID()
         << "   srcID"       << setw(11) << pkt->getHeader()->getSrcID() 
         << "   dstID"       << setw(11) << pkt->getHeader()->getDstID() 
         << "   preID"       << setw(11) << pkt->getHeader()->getPreID()
         << "   nexID"       << setw(11) << pkt->getHeader()->getNexID()
         << "   "            << pkt->type()
         << pkt->addition_information()
         //<< "   msg"         << setw(11) << dynamic_cast<IoT_data_payload*>(pkt->getPayload())->getMsg()
         << endl;
    // cout << pkt->type()
    //      << "   time "       << setw(11) << event::getCurTime() 
    //      << "   senID "      << setw(11) << senderID
    //      << "   pktID"       << setw(11) << pkt->getPacketID()
    //      << "   srcID "      << setw(11) << pkt->getHeader()->getSrcID() 
    //      << "   dstID"       << setw(11) << pkt->getHeader()->getDstID() 
    //      << "   preID"       << setw(11) << pkt->getHeader()->getPreID()
    //      << "   nexID"       << setw(11) << pkt->getHeader()->getNexID()
    //      << endl;
}

////////////////////////////////////////////////////////////////////////////////

class IoT_data_pkt_gen_event: public event {
    public:
        class gen_data; // forward declaration
            
    private:
        IoT_data_pkt_gen_event (IoT_data_pkt_gen_event &){}
        IoT_data_pkt_gen_event (){} // we don't allow users to new a IoT_data_pkt_gen_event by themselves
        // this constructor cannot be directly called by users; only by generator
        unsigned int src; // the src
        unsigned int dst; // the dst 
        // packet *pkt; // the packet
        string msg;
    
    protected:
        IoT_data_pkt_gen_event (unsigned int _trigger_time, void *data): event(_trigger_time), src(BROADCAST_ID), dst(BROADCAST_ID){
            pkt_gen_data * data_ptr = (pkt_gen_data*) data;
            src = data_ptr->src_id;
            dst = data_ptr->dst_id;
            // pkt = data_ptr->_pkt;
            msg = data_ptr->msg;
        } 
        
    public:
        virtual ~IoT_data_pkt_gen_event(){}
        // IoT_data_pkt_gen_event will trigger the packet gen function
        virtual void trigger();
        
        unsigned int event_priority() const;
        
        class IoT_data_pkt_gen_event_generator;
        friend class IoT_data_pkt_gen_event_generator;
        // IoT_data_pkt_gen_event_generator is derived from event_generator to generate an event
        class IoT_data_pkt_gen_event_generator : public event_generator{
                static IoT_data_pkt_gen_event_generator sample;
                // this constructor is only for sample to register this event type
                IoT_data_pkt_gen_event_generator() { /*cout << "send_event registered" << endl;*/ register_event_type(&sample); }
            protected:
                virtual event * generate(unsigned int _trigger_time, void *data){ 
                    // cout << "send_event generated" << endl; 
                    return new IoT_data_pkt_gen_event(_trigger_time, data); 
                }
            
            public:
                virtual string type() { return "IoT_data_pkt_gen_event";}
                ~IoT_data_pkt_gen_event_generator(){}
        };
        // this class is used to initialize the IoT_data_pkt_gen_event
        class pkt_gen_data{
            public:
                unsigned int src_id;
                unsigned int dst_id;
                string msg;
                // packet *_pkt;
        };
        
        void print () const;
};
IoT_data_pkt_gen_event::IoT_data_pkt_gen_event_generator IoT_data_pkt_gen_event::IoT_data_pkt_gen_event_generator::sample;

void IoT_data_pkt_gen_event::trigger() {
    if (node::id_to_node(src) == nullptr){
        cerr << "IoT_data_pkt_gen_event error: no node " << src << "!" << endl;
        return ;
    }
    else if ( dst != BROADCAST_ID && node::id_to_node(dst) == nullptr ) {
        cerr << "IoT_data_pkt_gen_event error: no node " << dst << "!" << endl;
        return;
    }
    
    IoT_data_packet *pkt = dynamic_cast<IoT_data_packet*> ( packet::packet_generator::generate("IoT_data_packet") );
    if (pkt == nullptr) { 
        cerr << "packet type is incorrect" << endl; return; 
    }
    IoT_data_header *hdr = dynamic_cast<IoT_data_header*> ( pkt->getHeader() );
    IoT_data_payload *pld = dynamic_cast<IoT_data_payload*> ( pkt->getPayload() );
    
    if (hdr == nullptr) {
        cerr << "header type is incorrect" << endl; return ;
    }
    if (pld == nullptr) {
        cerr << "payload type is incorrect" << endl; return ;
    }

    hdr->setSrcID(src);
    hdr->setDstID(dst);
    hdr->setPreID(src); // this column is not important when the packet is first received by the src (i.e., just generated)
    hdr->setNexID(src); // this column is not important when the packet is first received by the src (i.e., just generated)

    pld->setMsg(msg);
    
    recv_event::recv_data e_data;
    e_data.s_id = src;
    e_data.r_id = src; // to make the packet start from the src
    e_data._pkt = pkt;
    
    recv_event *e = dynamic_cast<recv_event*> ( event::event_generator::generate("recv_event", trigger_time, (void *)&e_data) );

}
unsigned int IoT_data_pkt_gen_event::event_priority() const {
    string string_for_hash;
    string_for_hash = to_string(getTriggerTime()) + to_string(src) + to_string (dst) ; //to_string (pkt->getPacketID());
    return get_hash_value(string_for_hash);
}
// the IoT_data_pkt_gen_event::print() function is used for log file
void IoT_data_pkt_gen_event::print () const {
    cout << "time "          << setw(11) << event::getCurTime() 
         << "        "       << setw(11) << " "
         << "        "       << setw(11) << " "
         << "   srcID"       << setw(11) << src
         << "   dstID"       << setw(11) << dst
         << "        "       << setw(11) << " "
         << "        "       << setw(11) << " "
         << "   IoT_data_packet generating"
         << endl;
}

class IoT_ctrl_pkt_gen_event: public event {
    public:
        class gen_data; // forward declaration
            
    private:
        IoT_ctrl_pkt_gen_event (IoT_ctrl_pkt_gen_event &){}
        IoT_ctrl_pkt_gen_event (){} // we don't allow users to new a IoT_ctrl_pkt_gen_event by themselves
        // this constructor cannot be directly called by users; only by generator
        unsigned int src; // the src
        unsigned int dst; // the dst 
        // unsigned int mat;
        // unsigned int act;
        // packet *pkt; // the packet
        string msg;
        // double per; // percentage
    
    protected:
        IoT_ctrl_pkt_gen_event (unsigned int _trigger_time, void *data): event(_trigger_time), src(BROADCAST_ID), dst(BROADCAST_ID){
            pkt_gen_data * data_ptr = (pkt_gen_data*) data;
            src = data_ptr->src_id;
            dst = data_ptr->dst_id;
            // pkt = data_ptr->_pkt;
            // mat = data_ptr->mat_id;
            // act = data_ptr->act_id;
            msg = data_ptr->msg;
            // per = data_ptr->per;
        } 
        
    public:
        virtual ~IoT_ctrl_pkt_gen_event(){}
        // IoT_ctrl_pkt_gen_event will trigger the packet gen function
        virtual void trigger();
        
        unsigned int event_priority() const;
        
        class IoT_ctrl_pkt_gen_event_generator;
        friend class IoT_ctrl_pkt_gen_event_generator;
        // IoT_ctrl_pkt_gen_event_generator is derived from event_generator to generate an event
        class IoT_ctrl_pkt_gen_event_generator : public event_generator{
                static IoT_ctrl_pkt_gen_event_generator sample;
                // this constructor is only for sample to register this event type
                IoT_ctrl_pkt_gen_event_generator() { /*cout << "send_event registered" << endl;*/ register_event_type(&sample); }
            protected:
                virtual event * generate(unsigned int _trigger_time, void *data){ 
                    // cout << "send_event generated" << endl; 
                    return new IoT_ctrl_pkt_gen_event(_trigger_time, data); 
                }
            
            public:
                virtual string type() { return "IoT_ctrl_pkt_gen_event";}
                ~IoT_ctrl_pkt_gen_event_generator(){}
        };
        // this class is used to initialize the IoT_ctrl_pkt_gen_event
        class pkt_gen_data{
            public:
                unsigned int src_id; // the sink
                unsigned int dst_id; // the node that should update its rule
                // unsigned int mat_id; // the target of the rule
                // unsigned int act_id; // the next hop toward the target recorded in the rule
                string msg;
                // double per; // the percentage
                // packet *_pkt;
        };
        
        void print () const;
};
IoT_ctrl_pkt_gen_event::IoT_ctrl_pkt_gen_event_generator IoT_ctrl_pkt_gen_event::IoT_ctrl_pkt_gen_event_generator::sample;

void IoT_ctrl_pkt_gen_event::trigger() {
    
    IoT_ctrl_packet *pkt = dynamic_cast<IoT_ctrl_packet*> ( packet::packet_generator::generate("IoT_ctrl_packet") );
    if (pkt == nullptr) { 
        cerr << "packet type is incorrect" << endl; return; 
    }
    IoT_ctrl_header *hdr = dynamic_cast<IoT_ctrl_header*> ( pkt->getHeader() );
    IoT_ctrl_payload *pld = dynamic_cast<IoT_ctrl_payload*> ( pkt->getPayload() );
    
    if (hdr == nullptr) {
        cerr << "header type is incorrect" << endl; return ;
    }
    if (pld == nullptr) {
        cerr << "payload type is incorrect" << endl; return ;
    }

    hdr->setSrcID(src); 
    hdr->setDstID(dst);
    hdr->setPreID(src);
    hdr->setNexID(src);
    
    // payload
    pld->setMsg(msg);
    // pld->setMatID(mat);
    // pld->setActID(act);
    // pld->setPer(per);
    
    recv_event::recv_data e_data;
    e_data.s_id = src;
    e_data.r_id = src;
    e_data._pkt = pkt;
    
    recv_event *e = dynamic_cast<recv_event*> ( event::event_generator::generate("recv_event",trigger_time, (void *)&e_data) );
}
unsigned int IoT_ctrl_pkt_gen_event::event_priority() const {
    string string_for_hash;
    // string_for_hash = to_string(getTriggerTime()) + to_string(src) + to_string(dst) + to_string(mat) + to_string(act); //to_string (pkt->getPacketID());
    string_for_hash = to_string(getTriggerTime()) + to_string(src) + to_string(dst) ; //to_string (pkt->getPacketID());
    return get_hash_value(string_for_hash);
}
// the IoT_ctrl_pkt_gen_event::print() function is used for log file
void IoT_ctrl_pkt_gen_event::print () const {
    cout << "time "          << setw(11) << event::getCurTime() 
         << "        "       << setw(11) << " "
         << "        "       << setw(11) << " "
         << "   srcID"       << setw(11) << src
         << "   dstID"       << setw(11) << dst
         << "        "       << setw(11) << " "
         << "        "       << setw(11) << " "
        //  << "   matID"       << setw(11) << mat
        //  << "   actID"       << setw(11) << act
         << "   IoT_ctrl_packet generating"
         << endl;
}



class AGG_ctrl_pkt_gen_event: public event {
    public:
        class gen_data; // forward declaration
            
    private:
        AGG_ctrl_pkt_gen_event (AGG_ctrl_pkt_gen_event &){}
        AGG_ctrl_pkt_gen_event (){} // we don't allow users to new a AGG_ctrl_pkt_gen_event by themselves
        // this constructor cannot be directly called by users; only by generator
        unsigned int src; // the src
        unsigned int dst; // the dst 
        // unsigned int mat;
        // unsigned int act;
        // packet *pkt; // the packet
        string msg;
        // double per; // percentage
    
    protected:
        AGG_ctrl_pkt_gen_event (unsigned int _trigger_time, void *data): event(_trigger_time), src(BROADCAST_ID), dst(BROADCAST_ID){
            pkt_gen_data * data_ptr = (pkt_gen_data*) data;
            src = data_ptr->src_id;
            dst = data_ptr->dst_id;
            // pkt = data_ptr->_pkt;
            // mat = data_ptr->mat_id;
            // act = data_ptr->act_id;
            msg = data_ptr->msg;
            // per = data_ptr->per;
        } 
        
    public:
        virtual ~AGG_ctrl_pkt_gen_event(){}
        // AGG_ctrl_pkt_gen_event will trigger the packet gen function
        virtual void trigger();
        
        unsigned int event_priority() const;
        
        class AGG_ctrl_pkt_gen_event_generator;
        friend class AGG_ctrl_pkt_gen_event_generator;
        // AGG_ctrl_pkt_gen_event_generator is derived from event_generator to generate an event
        class AGG_ctrl_pkt_gen_event_generator : public event_generator{
                static AGG_ctrl_pkt_gen_event_generator sample;
                // this constructor is only for sample to register this event type
                AGG_ctrl_pkt_gen_event_generator() { /*cout << "send_event registered" << endl;*/ register_event_type(&sample); }
            protected:
                virtual event * generate(unsigned int _trigger_time, void *data){ 
                    // cout << "send_event generated" << endl; 
                    return new AGG_ctrl_pkt_gen_event(_trigger_time, data); 
                }
            
            public:
                virtual string type() { return "AGG_ctrl_pkt_gen_event";}
                ~AGG_ctrl_pkt_gen_event_generator(){}
        };
        // this class is used to initialize the AGG_ctrl_pkt_gen_event
        class pkt_gen_data{
            public:
                unsigned int src_id; // the sink
                unsigned int dst_id; // the node that should update its rule
                // unsigned int mat_id; // the target of the rule
                // unsigned int act_id; // the next hop toward the target recorded in the rule
                string msg;
                // double per; // the percentage
                // packet *_pkt;
        };
        
        void print () const;
};
AGG_ctrl_pkt_gen_event::AGG_ctrl_pkt_gen_event_generator AGG_ctrl_pkt_gen_event::AGG_ctrl_pkt_gen_event_generator::sample;

void AGG_ctrl_pkt_gen_event::trigger() {
    
    AGG_ctrl_packet *pkt = dynamic_cast<AGG_ctrl_packet*> ( packet::packet_generator::generate("AGG_ctrl_packet") );
    if (pkt == nullptr) { 
        cerr << "packet type is incorrect" << endl; return; 
    }
    AGG_ctrl_header *hdr = dynamic_cast<AGG_ctrl_header*> ( pkt->getHeader() );
    AGG_ctrl_payload *pld = dynamic_cast<AGG_ctrl_payload*> ( pkt->getPayload() );
    
    if (hdr == nullptr) {
        cerr << "header type is incorrect" << endl; return ;
    }
    if (pld == nullptr) {
        cerr << "payload type is incorrect" << endl; return ;
    }

    hdr->setSrcID(src); 
    hdr->setDstID(dst);
    hdr->setPreID(src);
    hdr->setNexID(src);
    
    // payload
    pld->setMsg(msg);
    // pld->setMatID(mat);
    // pld->setActID(act);
    // pld->setPer(per);
    
    recv_event::recv_data e_data;
    e_data.s_id = src;
    e_data.r_id = src;
    e_data._pkt = pkt;
    
    recv_event *e = dynamic_cast<recv_event*> ( event::event_generator::generate("recv_event",trigger_time, (void *)&e_data) );
}
unsigned int AGG_ctrl_pkt_gen_event::event_priority() const {
    string string_for_hash;
    // string_for_hash = to_string(getTriggerTime()) + to_string(src) + to_string(dst) + to_string(mat) + to_string(act); //to_string (pkt->getPacketID());
    string_for_hash = to_string(getTriggerTime()) + to_string(src) + to_string(dst) ; //to_string (pkt->getPacketID());
    return get_hash_value(string_for_hash);
}
// the AGG_ctrl_pkt_gen_event::print() function is used for log file
void AGG_ctrl_pkt_gen_event::print () const {
    cout << "time "          << setw(11) << event::getCurTime() 
         << "        "       << setw(11) << " "
         << "        "       << setw(11) << " "
         << "   srcID"       << setw(11) << src
         << "   dstID"       << setw(11) << dst
         << "        "       << setw(11) << " "
         << "        "       << setw(11) << " "
        //  << "   matID"       << setw(11) << mat
        //  << "   actID"       << setw(11) << act
         << "   AGG_ctrl_packet generating"
         << endl;
}


class DIS_ctrl_pkt_gen_event: public event {
    public:
        class gen_data; // forward declaration
            
    private:
        DIS_ctrl_pkt_gen_event (DIS_ctrl_pkt_gen_event &){}
        DIS_ctrl_pkt_gen_event (){} // we don't allow users to new a DIS_ctrl_pkt_gen_event by themselves
        // this constructor cannot be directly called by users; only by generator
        unsigned int src; // the src
        unsigned int dst; // the dst 
        // unsigned int mat;
        // unsigned int act;
        // packet *pkt; // the packet
        string msg;
        // double per; // percentage
        int parent;
    
    protected:
        DIS_ctrl_pkt_gen_event (unsigned int _trigger_time, void *data): event(_trigger_time), src(BROADCAST_ID), dst(BROADCAST_ID){
            pkt_gen_data * data_ptr = (pkt_gen_data*) data;
            src = data_ptr->src_id;
            dst = data_ptr->dst_id;
            parent = data_ptr->parent;
            // pkt = data_ptr->_pkt;
            // mat = data_ptr->mat_id;
            // act = data_ptr->act_id;
            msg = data_ptr->msg;
            // per = data_ptr->per;
        } 
        
    public:
        virtual ~DIS_ctrl_pkt_gen_event(){}
        // DIS_ctrl_pkt_gen_event will trigger the packet gen function
        virtual void trigger();
        
        unsigned int event_priority() const;
        
        class DIS_ctrl_pkt_gen_event_generator;
        friend class DIS_ctrl_pkt_gen_event_generator;
        // DIS_ctrl_pkt_gen_event_generator is derived from event_generator to generate an event
        class DIS_ctrl_pkt_gen_event_generator : public event_generator{
                static DIS_ctrl_pkt_gen_event_generator sample;
                // this constructor is only for sample to register this event type
                DIS_ctrl_pkt_gen_event_generator() { /*cout << "send_event registered" << endl;*/ register_event_type(&sample); }
            protected:
                virtual event * generate(unsigned int _trigger_time, void *data){ 
                    // cout << "send_event generated" << endl; 
                    return new DIS_ctrl_pkt_gen_event(_trigger_time, data); 
                }
            
            public:
                virtual string type() { return "DIS_ctrl_pkt_gen_event";}
                ~DIS_ctrl_pkt_gen_event_generator(){}
        };
        // this class is used to initialize the DIS_ctrl_pkt_gen_event
        class pkt_gen_data{
            public:
                unsigned int src_id; // the sink
                unsigned int dst_id; // the node that should update its rule
                // unsigned int mat_id; // the target of the rule
                // unsigned int act_id; // the next hop toward the target recorded in the rule
                unsigned int parent;
                string msg;
                // double per; // the percentage
                // packet *_pkt;
        };
        
        void print () const;
};
DIS_ctrl_pkt_gen_event::DIS_ctrl_pkt_gen_event_generator DIS_ctrl_pkt_gen_event::DIS_ctrl_pkt_gen_event_generator::sample;

void DIS_ctrl_pkt_gen_event::trigger() {
    
    DIS_ctrl_packet *pkt = dynamic_cast<DIS_ctrl_packet*> ( packet::packet_generator::generate("DIS_ctrl_packet") );
    if (pkt == nullptr) { 
        cerr << "packet type is incorrect" << endl; return; 
    }
    DIS_ctrl_header *hdr = dynamic_cast<DIS_ctrl_header*> ( pkt->getHeader() );
    DIS_ctrl_payload *pld = dynamic_cast<DIS_ctrl_payload*> ( pkt->getPayload() );
    
    if (hdr == nullptr) {
        cerr << "header type is incorrect" << endl; return ;
    }
    if (pld == nullptr) {
        cerr << "payload type is incorrect" << endl; return ;
    }

    hdr->setSrcID(src); 
    hdr->setDstID(dst);
    hdr->setPreID(src);
    hdr->setNexID(src);
    
    // payload
    pld->setParent(parent);
    pld->setMsg(msg);
    // pld->setMatID(mat);
    // pld->setActID(act);
    // pld->setPer(per);
    
    recv_event::recv_data e_data;
    e_data.s_id = src;
    e_data.r_id = src;
    e_data._pkt = pkt;
    
    recv_event *e = dynamic_cast<recv_event*> ( event::event_generator::generate("recv_event",trigger_time, (void *)&e_data) );
}
unsigned int DIS_ctrl_pkt_gen_event::event_priority() const {
    string string_for_hash;
    // string_for_hash = to_string(getTriggerTime()) + to_string(src) + to_string(dst) + to_string(mat) + to_string(act); //to_string (pkt->getPacketID());
    string_for_hash = to_string(getTriggerTime()) + to_string(src) + to_string(dst) ; //to_string (pkt->getPacketID());
    return get_hash_value(string_for_hash);
}
// the DIS_ctrl_pkt_gen_event::print() function is used for log file
void DIS_ctrl_pkt_gen_event::print () const {
    cout << "time "          << setw(11) << event::getCurTime() 
         << "        "       << setw(11) << " "
         << "        "       << setw(11) << " "
         << "   srcID"       << setw(11) << src
         << "   dstID"       << setw(11) << dst
         << "        "       << setw(11) << " "
         << "        "       << setw(11) << " "
        //  << "   matID"       << setw(11) << mat
        //  << "   actID"       << setw(11) << act
         << "   DIS_ctrl_packet generating"
         << endl;
}

////////////////////////////////////////////////////////////////////////////////

class link {
        // all links created in the program
        static map< pair<unsigned int,unsigned int>, link*> id_id_link_table;
        
        unsigned int id1; // from
        unsigned int id2; // to
        
    protected:
        link(link&){} // this constructor should not be used
        link(){} // this constructor should not be used
        link(unsigned int _id1, unsigned int _id2): id1(_id1), id2(_id2) { id_id_link_table[pair<unsigned int,unsigned int>(id1,id2)] = this; }

    public:
        virtual ~link() { 
            id_id_link_table.erase (pair<unsigned int,unsigned int>(id1,id2)); // erase the link
        }
        
        static link * id_id_to_link (unsigned int _id1, unsigned int _id2) { 
            return ((id_id_link_table.find(pair<unsigned int,unsigned int>(_id1,_id2))!=id_id_link_table.end()) ? id_id_link_table[pair<unsigned,unsigned>(_id1,_id2)]: nullptr) ; 
        }

        virtual double getLatency() = 0; // you must implement your own latency
        
        static void del_link (unsigned int _id1, unsigned int _id2) {
            pair<unsigned int, unsigned int> temp;
            if (id_id_link_table.find(temp)!=id_id_link_table.end())
                id_id_link_table.erase(temp); 
        }

        static unsigned int getLinkNum () { return id_id_link_table.size(); }

        class link_generator {
                // lock the copy constructor
                link_generator(link_generator &){}
                // store all possible types of link
                static map<string,link_generator*> prototypes;
            protected:
                // allow derived class to use it
                link_generator() {}
                // after you create a new link type, please register the factory of this link type by this function
                void register_link_type(link_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your link
                virtual link* generate(unsigned int _id1, unsigned int _id2) = 0;
            public:
                // you have to implement your own type() to return your link type
        	    virtual string type() = 0;
        	    // this function is used to generate any type of link derived
        	    static link * generate (string type, unsigned int _id1, unsigned int _id2) {
        	        if(id_id_link_table.find(pair<unsigned int,unsigned int>(_id1,_id2))!=id_id_link_table.end()){
        	            std::cerr << "duplicate link id" << std::endl; // link id is duplicated
        	            return nullptr;
        	        }
        	        else if ( BROADCAST_ID == _id1 || BROADCAST_ID == _id2 ) {
        	            cerr << "BROADCAST_ID cannot be used" << endl;
        	            return nullptr;
        	        }
            		else if (prototypes.find(type) != prototypes.end()){ // if this type derived exists 
            		    link * created_link = prototypes[type]->generate(_id1,_id2);
            			return created_link; // generate it!!
            		}
            		std::cerr << "no such link type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered link types: " << endl;
            	    for (map<string,link::link_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~link_generator(){};
        };
};
map<string,link::link_generator*> link::link_generator::prototypes;
map<pair<unsigned int,unsigned int>, link*> link::id_id_link_table;

void node::add_phy_neighbor (unsigned int _id, string link_type){
    if (id == _id) return; // if the two nodes are the same...
    if (id_node_table.find(_id)==id_node_table.end()) return; // if this node does not exist
    if (phy_neighbors.find(_id)!=phy_neighbors.end()) return; // if this neighbor has been added
    phy_neighbors[_id] = true;
    
    link::link_generator::generate(link_type,id,_id);
}
void node::del_phy_neighbor (unsigned int _id){
    phy_neighbors.erase(_id);
    
}


class simple_link: public link {
    protected:
        simple_link() {} // it should not be used outside the class
        simple_link(simple_link&) {} // it should not be used
        simple_link(unsigned int _id1, unsigned int _id2): link (_id1,_id2){} // this constructor cannot be directly called by users
    
    public:
        virtual ~simple_link() {}
        virtual double getLatency() { return ONE_HOP_DELAY; } // you can implement your own latency
        
        class simple_link_generator;
        friend class simple_link_generator;
        // simple_link is derived from link_generator to generate a link
        class simple_link_generator : public link_generator {
                static simple_link_generator sample;
                // this constructor is only for sample to register this link type
                simple_link_generator() { /*cout << "simple_link registered" << endl;*/ register_link_type(&sample); }
            protected:
                virtual link * generate(unsigned int _id1, unsigned int _id2) 
                { /*cout << "simple_link generated" << endl;*/ return new simple_link(_id1,_id2); }
            public:
                virtual string type() { return "simple_link"; }
                ~simple_link_generator(){}
        };
};

simple_link::simple_link_generator simple_link::simple_link_generator::sample;


// the IoT_data_packet_event function is used to add an initial event
void IoT_data_packet_event (unsigned int src, unsigned int dst=0, unsigned int t = 0, string msg = "default"){
    if ( node::id_to_node(src) == nullptr || (dst != BROADCAST_ID && node::id_to_node(dst) == nullptr) ) {
        cerr << "src or dst is incorrect" << endl; return ;
        return;
    }

    IoT_data_pkt_gen_event::pkt_gen_data e_data;
    e_data.src_id = src;
    e_data.dst_id = dst;
    e_data.msg = msg;
    
    // recv_event *e = dynamic_cast<recv_event*> ( event::event_generator::generate("recv_event",t, (void *)&e_data) );
    IoT_data_pkt_gen_event *e = dynamic_cast<IoT_data_pkt_gen_event*> ( event::event_generator::generate("IoT_data_pkt_gen_event",t, (void *)&e_data) );
    if (e == nullptr) cerr << "event type is incorrect" << endl;
}

// the IoT_ctrl_packet_event function is used to add an initial event

void IoT_ctrl_packet_event (unsigned int src = 0, unsigned int t = event::getCurTime(),
                        string msg = "default") {
        // 1st parameter: the source; the destination that want to broadcast a msg with counter 0 (i.e., match ID)
        // 2nd parameter: time (optional)
        // 3rd parameter: msg (optional)
    if ( node::id_to_node(src) == nullptr ) {
        cerr << "id is incorrect" << endl; return;
    }
    
    // unsigned int src = con_id;
    IoT_ctrl_pkt_gen_event::pkt_gen_data e_data;
    e_data.src_id = src;
    e_data.dst_id = BROADCAST_ID;
    // e_data.mat_id = mat;
    // e_data.act_id = act;
    e_data.msg = msg;
    // e_data.per = per;
    
    IoT_ctrl_pkt_gen_event *e = dynamic_cast<IoT_ctrl_pkt_gen_event*> ( event::event_generator::generate("IoT_ctrl_pkt_gen_event",t, (void *)&e_data) );
    if (e == nullptr) cerr << "event type is incorrect" << endl;
}

// the AGG_ctrl_packet_event function is used to add an initial event
void AGG_ctrl_packet_event (unsigned int src, unsigned int dst=0, unsigned int t = event::getCurTime(), string msg = "default"){
    if ( node::id_to_node(src) == nullptr || (dst != BROADCAST_ID && node::id_to_node(dst) == nullptr) ) {
        cerr << "src or dst is incorrect" << endl; return ;
        return;
    }

    AGG_ctrl_pkt_gen_event::pkt_gen_data e_data;
    e_data.src_id = src;
    e_data.dst_id = dst;
    e_data.msg = msg;
    
    // recv_event *e = dynamic_cast<recv_event*> ( event::event_generator::generate("recv_event",t, (void *)&e_data) );
    AGG_ctrl_pkt_gen_event *e = dynamic_cast<AGG_ctrl_pkt_gen_event*> ( event::event_generator::generate("AGG_ctrl_pkt_gen_event",t, (void *)&e_data) );
    if (e == nullptr) cerr << "event type is incorrect" << endl;
}

// the DIS_ctrl_packet_event function is used to add an initial event
void DIS_ctrl_packet_event (unsigned int sink_id = 0, unsigned int t = event::getCurTime(), string msg = "default"){
    if ( node::id_to_node(sink_id) == nullptr ) {
        cerr << "sink_id or id is incorrect" << endl; return ;
        return;
    }

    DIS_ctrl_pkt_gen_event::pkt_gen_data e_data;
    e_data.src_id = sink_id;
    e_data.dst_id = sink_id;
    e_data.parent = sink_id;
    e_data.msg = msg;
    
    // recv_event *e = dynamic_cast<recv_event*> ( event::event_generator::generate("recv_event",t, (void *)&e_data) );
    DIS_ctrl_pkt_gen_event *e = dynamic_cast<DIS_ctrl_pkt_gen_event*> ( event::event_generator::generate("DIS_ctrl_pkt_gen_event",t, (void *)&e_data) );
    if (e == nullptr) cerr << "event type is incorrect" << endl;
}

// send_handler function is used to transmit packet p based on the information in the header
// Note that the packet p will not be discard after send_handler ()

void node::send_handler(packet *p){
    packet *_p = packet::packet_generator::replicate(p);
    send_event::send_data e_data;
    e_data.s_id = _p->getHeader()->getPreID();
    e_data.r_id = _p->getHeader()->getNexID();
    e_data._pkt = _p;
    send_event *e = dynamic_cast<send_event*> (event::event_generator::generate("send_event",event::getCurTime(), (void *)&e_data) );
    if (e == nullptr) cerr << "event type is incorrect" << endl;
}

void node::send(packet *p){ // this function is called by event; not for the user
    if (p == nullptr) return;
    
    unsigned int _nexID = p->getHeader()->getNexID();
    for ( map<unsigned int,bool>::iterator it = phy_neighbors.begin(); it != phy_neighbors.end(); it ++) {
        unsigned int nb_id = it->first; // neighbor id
        
        if (nb_id != _nexID && BROADCAST_ID != _nexID) continue; // this neighbor will not receive the packet
        
        unsigned int trigger_time = event::getCurTime() + link::id_id_to_link(id, nb_id)->getLatency() ; // we simply assume that the delay is fixed
        // cout << "node " << id << " send to node " <<  nb_id << endl;
        recv_event::recv_data e_data;
        e_data.s_id = id;    // set the sender   (i.e., preID)
        e_data.r_id = nb_id; // set the receiver (i.e., nexID)
        
        packet *p2 = packet::packet_generator::replicate(p);
        e_data._pkt = p2;
        
        recv_event *e = dynamic_cast<recv_event*> (event::event_generator::generate("recv_event", trigger_time, (void*) &e_data)); // send the packet to the neighbor
        if (e == nullptr) cerr << "event type is incorrect" << endl;
    }
    packet::discard(p);
}

//unsigned int to string
string vectorToString(const vector<unsigned int>& vec) {
    ostringstream oss;
    for (unsigned int i = 0; i < vec.size(); ++i) {
        oss << vec[i];
        if (i != vec.size() - 1) {
            oss << ", "; 
        }
    }
    return oss.str();
}

//sting to unsigned int
vector<unsigned int> stringToVector(const string& str) {
    vector<unsigned int> vec;
    if(str.size()==0){
        return vec;
    }
    istringstream iss(str);
    string temp;
    while (getline(iss, temp, ',')) {
        istringstream elem(temp);
        unsigned int num;
        elem >> num;
        vec.push_back(num);
    }
    return vec;
}

// you have to write the code in recv_handler of IoT_device
void IoT_device::recv_handler (packet *p){
    
    //initialize surround map
    if(flag){
        initialize_surround_map(getNodeNum());
        flag = 0;
    }
    

    // in this function, you are "not" allowed to use node::id_to_node(id) !!!!!!!!

    // this is a simple example
    // node 0 broadcasts its message to every node and every node relays the packet "only once" and increases its counter
    // the variable hi is used to examine whether the packet has been received by this node before
    // you can remove the variable hi and create your own routing table in class IoT_device
    if (p == nullptr) return;

    if (p->type() == "IoT_ctrl_packet") { // the device receives a packet from the sink
        IoT_ctrl_packet *p3 = nullptr;
        p3 = dynamic_cast<IoT_ctrl_packet*> (p);
        IoT_ctrl_payload *l3 = nullptr;
        l3 = dynamic_cast<IoT_ctrl_payload*> (p3->getPayload());

        unsigned int Node_sent_packet = p3->getHeader()->getPreID();
        unsigned int Node_sent_packet_parent = p3->getHeader()->getParent();
        unsigned int Node_sent_packet_counter = p3->getHeader()->getCounter();


        if(l3->getCounter()<counter){
            hi = false;
        } 
        if(p->getHeader()->getPreID()<GetParent()&&l3->getCounter()==counter){
            hi = false;
        }
        
        if(hi && FindConnect(p->getHeader()->getPreID())){
            hi = true;
        }
        if(l3->getCounter()>counter){
            hi = true;
        }

        int flag = 0;
        //compare surround counter
        if(Smallest_Counter_Id[Node_sent_packet]>Node_sent_packet_counter){
            Smallest_Counter_Id[Node_sent_packet] = Node_sent_packet_counter;
            Smallest_Parent_Id[Node_sent_packet] = Node_sent_packet_parent;
            if(Node_sent_packet_parent==getNodeID()){
                flag = 1;
            }
        }
        else if(Smallest_Counter_Id[Node_sent_packet]==Node_sent_packet_counter&&Smallest_Parent_Id[Node_sent_packet]>Node_sent_packet_parent){
            Smallest_Parent_Id[Node_sent_packet] = Node_sent_packet_parent;
            if(Node_sent_packet_parent==getNodeID()){
                flag = 1;
            }
        }
        
        //Delete those child choosing other parent
        if(FindChild(Node_sent_packet)){
            if(Smallest_Parent_Id[Node_sent_packet]!=getNodeID ()){
                DeleChild(Node_sent_packet);        
            }
        }
        
        //if the packet counter or id smaller then record update
        if(flag){
            AddChild(Node_sent_packet);

            hi = true;
        }

        if(!hi){
            
            counter = l3->getCounter();

            p3->getHeader()->setParent(p->getHeader()->getPreID());//save their parent
            p3->getHeader()->setOldparent(GetParent()); // if there is no old parent which is 99999
            
            SetParent(p3->getHeader()->getPreID()); // set new parent
            
            const map<unsigned int, bool> &nblist = getPhyNeighbors();
            for (map<unsigned int, bool>::const_iterator it = nblist.begin(); it != nblist.end(); it++)
            {
                if(it->second){
                    AddConnect(it->first);
                }
            }

            //using when agg ctrl packet to construct graph
            if(AggMsg=="default"){
                AddConnect(getNodeID());//add self node at last num to use as agg msg
                AggMsg = vectorToString(GetPhyConnect());
                DeleConnect();//dele self node
            }
            
            //Org Broadcast
            p3->getHeader()->setPreID ( getNodeID() );
            p3->getHeader()->setNexID ( BROADCAST_ID );
            p3->getHeader()->setDstID ( BROADCAST_ID );
            p3->getHeader()->setCounter ( counter );
            l3->increase();
            hi = true;
            send_handler(p3);

        } 
        //unsigned mat = l3->getMatID();
        // unsigned act = l3->getActID();
        // string msg = l3->getMsg(); // get the msg
    }
    else if (p->type() == "IoT_data_packet" ) { // the device receives a packet    
        
        IoT_data_packet *p3 = nullptr;
        p3 = dynamic_cast<IoT_data_packet*> (p);
        if(IsThereChild()){
            //dele the child to record whether get each child packer
            DeleChild(p3->getHeader()->getPreID());
        }
        if(!IsThereChild()){
            p3->getHeader()->setPreID ( getNodeID() );
            p3->getHeader()->setNexID ( GetParent());
            p3->getHeader()->setDstID ( 0 );
            //cout << "node " << getNodeID() << " send the packet" << endl;
            send_handler(p3);
        }
    }
    else if (p->type() == "AGG_ctrl_packet") {
        AGG_ctrl_packet *p3 = nullptr;
        p3 = dynamic_cast<AGG_ctrl_packet*> (p);
        AGG_ctrl_payload *l3 = nullptr;
        l3 = dynamic_cast<AGG_ctrl_payload*> (p3->getPayload());

        if((l3->getMsg()=="default")){
            p3->getHeader()->setCheatParent ( GetParent() );
            p3->getHeader()->setSrcID ( getNodeID() );
            p3->getHeader()->setCounter ( counter );
            p3->getHeader()->setData ( get_data_size() );
            l3->setMsg(AggMsg);
        }

        
        Reversed_Path_Map[p3->getHeader()->getSrcID()]=p3->getHeader()->getPreID();
        p3->getHeader()->setPreID ( getNodeID() );
        p3->getHeader()->setNexID ( GetParent() );
        p3->getHeader()->setDstID ( 0 );
        
        //p3->getHeader()->setAggregatedData ( p3->getHeader()->getAggregatedData()+get_data_size() );
        
        send_handler(p3);
        
        //cout << "node id = " << getNodeID() << ", msg = "  << l3->getMsg() << endl;
    }
    else if (p->type() == "DIS_ctrl_packet") {
        DIS_ctrl_packet *p3 = nullptr;
        p3 = dynamic_cast<DIS_ctrl_packet*> (p);
        DIS_ctrl_payload *l3 = nullptr;
        l3 = dynamic_cast<DIS_ctrl_payload*> (p3->getPayload());

        unsigned int dst_id=p3->getHeader()->getDstID();

        p3->getHeader()->setPreID ( getNodeID() );
        p3->getHeader()->setNexID ( Reversed_Path_Map[dst_id] );
        
        //cout << "node id = " << getNodeID() << ", parent = "  << l3->getParent() << endl;
        //cout << "dst id = " << dst_id << ", next = "  << Reversed_Path_Map[dst_id] << endl;
        if(Reversed_Path_Map[dst_id]!=getNodeID()){
            send_handler(p3);
        }
        else{
            
            parent=l3->getParent();

            
            DeleteAllChild();
            while( !p3->getHeader()->new_children.empty()){
                AddChild(p3->getHeader()->new_children.back());

                p3->getHeader()->new_children.pop_back();
                
            }

        }
    }

    // you should implement the OSPF algorithm in recv_handler
    // getNodeID() returns the id of the current node
    
    // The current node's neighbors are already stored in the following variable 
    // map<unsigned int,bool> node::phy_neighbors
    // however, this variable is private in the class node
    // You have to use node::getPhyNeighbors to get the variable
    // for example, if you want to print all the neighbors of this node
    // const map<unsigned int,bool> &nblist = getPhyNeighbors();
    // cout << "node " << getNodeID() << "'s nblist: ";
    // for (map<unsigned int,bool>::const_iterator it = nblist.begin(); it != nblist.end(); it ++) {
    //     cout << it->first << ", " ;
    // }
    // cout << endl;


    // you can use p->getHeader()->setSrcID() or getSrcID()
    //             p->getHeader()->setDstID() or getDstID()
    //             p->getHeader()->setPreID() or getPreID()
    //             p->getHeader()->setNexID() or getNexID() to change or read the packet header
    
    // In addition, you can get the packet, header, and payload with the correct type
    // in fact, this is downcasting
    // IoT_data_packet * pkt = dynamic_cast<IoT_data_packet*> (p);
    // IoT_data_header * hdr = dynamic_cast<IoT_data_header*> (p->getHeader());
    // IoT_data_payload * pld = dynamic_cast<IoT_data_payload*> (p->getPayload());
    
    // you can also change the IoT_data_payload setting
    // pld->setMsg(string): to set the message transmitted to the destination
    
    // Besides, you can use packet::packet_generator::generate() to generate a new packet; note that you should fill the header and payload in the packet
    // moreover, you can use "packet *p2 = packet::packet_generator::replicate(p)" to make a clone p2 of packet p
    // note that if the packet is generated or replicated manually, you must delete it by packet::discard() manually before recv_handler finishes
    
    // "IMPORTANT":
    // You have to "carefully" fill the correct information (e.g., srcID, dstID, ...) in the packet before you send it
    // Note that if you want to transmit a packet to only one next node (i.e., unicast), then you fill the ID of a specific node to "nexID" in the header
    // Otherwise, i.e., you want to broadcasts, then you fill "BROADCAST_ID" to "nexID" in the header
    // after that, you can use send() to transmit the packet 
    // usage: send_handler (p);
    
    // note that packet p will be discarded (deleted) after recv_handler(); you don't need to manually delete it
}

/*
// Function to convert the graph to a complete graph with weights generated by its original shortest path using Floyd-Warshall algorithm
map<pair<unsigned int, unsigned int>, int> convertToCompleteGraph(const map<unsigned int, vector<unsigned int>>& graph) {
    vector<unsigned int> vertices;
    for (const auto& node : graph) {
        vertices.push_back(node.first);
    }

    int V = vertices.size();
    unordered_map<unsigned int, unordered_map<unsigned int, int>> dist;

    // Initialize distances based on direct edges
    for (unsigned int u : vertices) {
        for (unsigned int v : vertices) {
            if (u == v) {
                dist[u][v] = 0;
            } else if (graph.at(u).end() != find(graph.at(u).begin(), graph.at(u).end(), v)) {
                dist[u][v] = 1;
            } else {
                dist[u][v] = INT_MAX;
            }
        }
    }

    // Apply Floyd-Warshall algorithm
    for (unsigned int k : vertices) {
        for (unsigned int i : vertices) {
            for (unsigned int j : vertices) {
                if (dist[i][k] < INT_MAX && dist[k][j] < INT_MAX) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    // Build the complete graph
    map<pair<unsigned int, unsigned int>, int> completeGraph;
    for (unsigned int u : vertices) {
        for (unsigned int v : vertices) {
            if (u != v) {
                completeGraph[{u, v}] = dist[u][v];
                completeGraph[{v, u}] = dist[u][v]; // Since the graph is undirected
            }
        }
    }

    return completeGraph;
}



// Function to print the complete graph with weights
void printCompleteGraph(const map<pair<unsigned int, unsigned int>, int>& completeGraph) {
    for (const auto& edge : completeGraph) {
        cout << "Edge (" << edge.first.first << ", " << edge.first.second << ") has weight " << edge.second << endl;
    }
}
*/






/*
//for last tree
typedef pair<int, int> pii;

class Graph {
    int V; // Number of vertices
    vector<vector<pii>> adj; // Adjacency list

public:
    Graph(int V);
    void addEdge(int u, int v, int w);
    void LAST(int src);
    void printLAST(const vector<int>& parent);
    int find(vector<int>& parent, int i);
    void Union(vector<int>& parent, vector<int>& rank, int x, int y);
    int kruskalMST();
};

// Graph constructor
Graph::Graph(int V) {
    this->V = V;
    adj.resize(V);
}

// Add edge to the graph
void Graph::addEdge(int u, int v, int w) {
    adj[u].push_back(make_pair(v, w));
    adj[v].push_back(make_pair(u, w));
}

// Find function for Kruskal's algorithm
int Graph::find(vector<int>& parent, int i) {
    if (parent[i] == i) {
        return i;
    }
    return parent[i] = find(parent, parent[i]);
}

// Union function for Kruskal's algorithm
void Graph::Union(vector<int>& parent, vector<int>& rank, int x, int y) {
    int xroot = find(parent, x);
    int yroot = find(parent, y);
    if (rank[xroot] < rank[yroot]) {
        parent[xroot] = yroot;
    } else if (rank[xroot] > rank[yroot]) {
        parent[yroot] = xroot;
    } else {
        parent[yroot] = xroot;
        rank[xroot]++;
    }
}

// Kruskal's algorithm to find MST weight
int Graph::kruskalMST() {
    vector<pair<int, pii>> edges;
    for (int u = 0; u < V; ++u) {
        for (auto &neighbor : adj[u]) {
            int v = neighbor.first;
            int w = neighbor.second;
            edges.push_back({w, {u, v}});
        }
    }
    sort(edges.begin(), edges.end());

    vector<int> parent(V);
    vector<int> rank(V, 0);
    for (int i = 0; i < V; ++i) {
        parent[i] = i;
    }

    int mst_weight = 0;
    for (auto &edge : edges) {
        int u = edge.second.first;
        int v = edge.second.second;
        int w = edge.first;
        int set_u = find(parent, u);
        int set_v = find(parent, v);
        if (set_u != set_v) {
            mst_weight += w;
            Union(parent, rank, set_u, set_v);
        }
    }
    return mst_weight;
}

// Print the LAST tree
void Graph::printLAST(const vector<int>& parent) {
    cout << "Edges in the 3-2 LAST:" << endl;
    for (int i = 0; i < V; ++i) {
        if (parent[i] != -1) {
            cout << parent[i] << " - " << i << endl;
        }
    }
}

// Light Approximate Shortest-path Tree (LAST) construction
void Graph::LAST(int src) {
    // Distance vector and priority queue for Dijkstra's algorithm
    vector<int> dist(V, INT_MAX);
    priority_queue<pii, vector<pii>, greater<pii>> pq;

    // Initialize distance to source node and push it to the queue
    dist[src] = 0;
    pq.push(make_pair(0, src));

    // Store parent of each node for tree construction
    vector<int> parent(V, -1);

    // Dijkstra's algorithm
    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        // Process each neighbor of u
        for (auto &neighbor : adj[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;

            // Relaxation step
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push(make_pair(dist[v], v));
                parent[v] = u;
            }
        }
    }

    // Degree constraint
    vector<int> degree(V, 0);

    // Construct the LAST with constraints
    for (int v = 0; v < V; ++v) {
        if (parent[v] != -1 && degree[parent[v]] < 3 && degree[v] < 3) {
            // Add edge to the tree if degree constraints are satisfied
            degree[parent[v]]++;
            degree[v]++;
        }
    }

    // Print the LAST
    printLAST(parent);

    // Ensure the distance ratio constraint
    for (int u = 0; u < V; ++u) {
        for (int v = 0; v < V; ++v) {
            if (u != v) {
                // Compute the distance between u and v in the LAST
                int last_distance = dist[u] + dist[v];
                // Compute the shortest path distance between u and v in the original graph
                int shortest_path_distance = dist[u];
                // Check the ratio constraint
                if (last_distance > 2 * shortest_path_distance) {
                    cout << "Ratio constraint violated for nodes " << u << " and " << v << endl;
                }
            }
        }
    }

    // Compare the LAST weight to the MST weight
    int last_weight = 0;
    for (int u = 0; u < V; ++u) {
        for (auto &neighbor : adj[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;
            if (parent[v] == u || parent[u] == v) {
                last_weight += weight;
            }
        }
    }
    int mst_weight = kruskalMST();
    cout << "LAST weight: " << last_weight << endl;
    cout << "MST weight: " << mst_weight << endl;
    if (last_weight > 2 * mst_weight) {
        cout << "Weight constraint violated" << endl;
    }
}
*/
/*
// Function to perform Dijkstra's algorithm
map<unsigned int, unsigned int> dijkstra(unsigned int source, const map<pair<unsigned int, unsigned int>, int> &edges, unsigned int num_nodes) {
    // Priority queue to store (distance, node) pairs
    priority_queue<pair<int, unsigned int>, vector<pair<int, unsigned int>>, greater<pair<int, unsigned int>>> pq;

    // Vector to store the minimum distance to each node
    vector<int> dist(num_nodes, numeric_limits<int>::max());
    dist[source] = 0;
    pq.push({0, source});

    // Map to store the parent of each node in the shortest path tree
    map<unsigned int, unsigned int> parent;
    parent[source] = source;

    while (!pq.empty()) {
        unsigned int u = pq.top().second;
        pq.pop();

        // Iterate over all edges outgoing from u
        for (unsigned int v = 0; v < num_nodes; ++v) {
            if (u != v && edges.count({u, v})) {
                int weight = edges.at({u, v});
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                    parent[v] = u;
                }
            }
        }
    }

    return parent;
}
*/

// Function to recursively calculate the aggregated size of each node's subtree
void calculateSubtreeSizes(unsigned int node, const unordered_map<unsigned int, std::vector<unsigned int>>& children, 
                           const map<unsigned int,unsigned int>& dataSize, vector<unsigned int>& subtreeSize) {
    // Start with the node's own data size
    subtreeSize[node] = dataSize.at(node);
    
    // Check if the node has children
    if (children.find(node) != children.end()) {
        // Add the sizes of the node's children
        for (unsigned int child : children.at(node)) {
            calculateSubtreeSizes(child, children, dataSize, subtreeSize);
            subtreeSize[node] += subtreeSize[child];
        }
    }
}

void IoT_sink::recv_handler (packet *p){

        // in this function, you are "not" allowed to use node::id_to_node(id) !!!!!!!!

    // this is a simple example
    // node 0 broadcasts its message to every node and every node relays the packet "only once" and increases its counter
    // the variable hi is used to examine whether the packet has been received by this node before
    // you can remove the variable hi and create your own routing table in class IoT_device
    if (p == nullptr) return ;


    
    if (p->type() == "IoT_ctrl_packet" && !hi ) { // the device receives a packet from the sink
        IoT_ctrl_packet *p3 = nullptr;
        p3 = dynamic_cast<IoT_ctrl_packet*> (p);
        IoT_ctrl_payload *l3 = nullptr;
        l3 = dynamic_cast<IoT_ctrl_payload*> (p3->getPayload());
        
        p3->getHeader()->setPreID ( getNodeID() );
        p3->getHeader()->setNexID ( BROADCAST_ID );
        p3->getHeader()->setDstID ( BROADCAST_ID );
        
        l3->increase();
        hi = true;
        send_handler(p3); 

        //unsigned mat = l3->getMatID();
        // unsigned act = l3->getActID();
        // string msg = l3->getMsg(); // get the msg
    }
    else if (p->type() == "IoT_data_packet" ) { // the device receives a packet
        //cout << "node " << getNodeID() << " get the packet" << endl;
    }
    else if (p->type() == "AGG_ctrl_packet") {
        AGG_ctrl_packet *p3 = nullptr;
        p3 = dynamic_cast<AGG_ctrl_packet*> (p);
        AGG_ctrl_payload *l3 = nullptr;
        l3 = dynamic_cast<AGG_ctrl_payload*> (p3->getPayload());

        new_parent[p3->getHeader()->getSrcID()]=p3->getHeader()->getCheatParent();

        Reversed_Path_Map[p3->getHeader()->getSrcID()]=p3->getHeader()->getPreID();
        
        vector<unsigned int> temp = stringToVector(l3->getMsg());
        int sent_from_who=temp.back();
        temp.pop_back();
       
        //to constrrct the whole graph
        while(!temp.empty()){
            graph[sent_from_who].push_back(temp.back());
            temp.pop_back();
        }
        
        //use record tree level
        level[p3->getHeader()->getSrcID()]=p3->getHeader()->getCounter();

        //use record data size
        data[p3->getHeader()->getSrcID()]=p3->getHeader()->getData();


        //cout << "node id = " << getNodeID() << ", msg = "  << l3->getMsg() << endl;
        
    }

    else if (p->type() == "DIS_ctrl_packet") {
        DIS_ctrl_packet *p3 = nullptr;
        p3 = dynamic_cast<DIS_ctrl_packet*> (p);
        DIS_ctrl_payload *l3 = nullptr;
        l3 = dynamic_cast<DIS_ctrl_payload*> (p3->getPayload());

        //add node 0 to and its neighbor to graph then get complete graph
        const map<unsigned int, bool> &nblist = getPhyNeighbors();
        for (map<unsigned int, bool>::const_iterator it = nblist.begin(); it != nblist.end(); it++)
        {
            if(it->second){
                graph[0].push_back(it->first);
            }
        }

        //construct aggregated data
        // Build the children list for each node
        unordered_map<unsigned int, vector<unsigned int>> children;
        for (const auto& pair : new_parent) {
            children[pair.second].push_back(pair.first);
        }
        data[0]=0;
        // Initialize the subtree size array
        vector<unsigned int> subtreeSize(getNodeNum(), 0);
        calculateSubtreeSizes(0, children, data, subtreeSize);
        /*
        for (unsigned int i = 1; i < subtreeSize.size(); ++i) {
            std::cout << "Node " << i << " aggregated data size: " << subtreeSize[i] << std::endl;
        }
        */

        //evaluate
        //find lowewr level
        int deepest=0;
        for(int i=0;i<getNodeNum();i++){
            if(level[i]>deepest){
                deepest=level[i];
            }
        }

        int nowlevel=deepest-1;

        for(nowlevel;nowlevel>0;nowlevel--){
            
            //evaluate target
            vector<int> target;
            for(int i=0;i<getNodeNum();i++){
                if(level[i]==nowlevel){
                    target.push_back(i);
                }
            }

            //evalueate aggregated size and packetsize
            while(!target.empty()){
                
                int node = target.back();
                int packet_size = get_packet_size();
                int parent = new_parent[node];
                
                target.pop_back();

                //if it is leaf
                if(subtreeSize[node]==data[node]){
                    continue;
                }
                
                //traverse the children
                for(auto it:children[node]){
                    //if chilren are leaf
                    if(data[it]==subtreeSize[it]){
                        double using_packet_num = ceil(subtreeSize[node]/packet_size);
                        double after_packet_num = ceil((subtreeSize[node]-data[it])/packet_size);
                        //if cut it can be better
                        if(using_packet_num>after_packet_num){
                            //find anyone can help him
                            for(auto node2:graph[it]){
                                //determine whether to move
                                int flag=1;
                                
                                //if cause the helper node send more packet skip
                                if(((subtreeSize[node2]+data[it])/packet_size)>subtreeSize[node2]/packet_size){
                                    flag=0;
                                    continue;
                                }

                                int par=new_parent[node2];
                                //it will also influnce the packet node aggregated data so need trace its parent until the node level
                                //and examine the parent will increase packet or not (>= tareget level)
                                //if increase then skip it
                                while(level[par]>=level[node]){
                                    
                                    //parent subtree size + data add size
                                    if(((subtreeSize[par]+data[it])/packet_size)>subtreeSize[par]/packet_size){
                                        flag=0;
                                        break;
                                    }
                                    
                                    par=new_parent[par];
                                }

                                //if transfer node is a deal
                                if(flag){
                                    subtreeSize[node2]+=data[it];
                                    subtreeSize[node]-=data[it];
                                    
                                    int par=new_parent[node2];
                                    new_parent[it]=node2;
                                    level[it]=level[node2]+1;

                                    //not same level
                                    if(level[node]!=level[node2]){
                                    
                                        //modify subtreesize and let par and node to the same level
                                        while(level[par]>=level[node]){

                                            subtreeSize[par]+=data[it];

                                            par=new_parent[par];
                                        }
                                        
                                        //we do not know par and node parent same or not
                                        //so keep trace and update subtree size there cross node
                                        int par2=new_parent[node];
                                        //?
                                        while(par!=par2){
                                            subtreeSize[par]+=data[it];
                                            subtreeSize[par2]-=data[it];
                                            par=new_parent[par];
                                            par2=new_parent[par2];
                                        }
                                        
                                    }
                                        
                                    

                                    
                                    
                                }
                                
                            }
                            
                        }
                    }
                     //if child is subtree , it need find same level helper

                    
                    else{
                        for(auto node2:graph[it]){
                            //make sure level not changing
                            if(level[node2]+1!=level[it]){
        
                                continue;
                            }

                            double org_packet_num = ceil(subtreeSize[node2]/packet_size)+ceil(subtreeSize[node]/packet_size);
                            double after_packet_num = ceil((subtreeSize[node2]+subtreeSize[it])/packet_size)+ceil((subtreeSize[node]-subtreeSize[it])/packet_size);
                            
                            
                            if(org_packet_num>after_packet_num){
                                //?
                                subtreeSize[node]-=subtreeSize[it];
                                subtreeSize[node2]+=subtreeSize[it];
                                new_parent[it]=node2;

                                //modiy parrent subtree size
                                int parentA=new_parent[node];
                                int parentB=new_parent[node2];

                                if(parentA==0||parentB==0){
                                    continue;
                                }
                                
                                
                                while(parentA!=parentB){
                                    subtreeSize[parentA]-=subtreeSize[it];
                                    subtreeSize[parentB]+=subtreeSize[it];
                                    parentA=new_parent[parentA];
                                    parentB=new_parent[parentB];
                                }

                                
                            }
                            
                            
                        }
                    }
                    
                    

                   
                }
            }
            
        }
        /*
        for (unsigned int i = 1; i < subtreeSize.size(); ++i) {
            cout << "Node " << i << " aggregated data size: " << subtreeSize[i] << endl;
        }
        */





        //printGraph();

        for(int id=1;id<getNodeNum();id++){
            l3->setParent(new_parent[id]);

            vector<int> new_children;

            //construct the new child
            for(int i=0;i<getNodeNum();i++){
                if(id==new_parent[i]){
                    new_children.push_back(i);
                }
            }



            p3->getHeader()->new_children.assign(new_children.begin(), new_children.end());



            p3->getHeader()->setSrcID ( 0 );
            p3->getHeader()->setPreID ( getNodeID() );
            p3->getHeader()->setNexID ( Reversed_Path_Map[id] );
            p3->getHeader()->setDstID ( id );

            send_handler(p3);

        }
        
        //cout << "node id = " << getNodeID() << ", parent = "  << l3->getParent() << endl;
    }
}

int main()
{
    // header::header_generator::print(); // print all registered headers
    // payload::payload_generator::print(); // print all registered payloads
    // packet::packet_generator::print(); // print all registered packets
    //node::node_generator::print();// print all registered nodes
    // event::event_generator::print(); // print all registered events
    // link::link_generator::print(); // print all registered links 
    
    // read the input and generate devices

    unsigned int Nodes, Links, PacketSize;
    int SimTime, BfsStartTime, AggStartTime;
    int DisStartTime, DataTransTime;
    int NodeId, DataSize;
    int LinkId, LinkEnd1, LinkEnd2;
    
    cin >> Nodes >> Links >> PacketSize;
    cin >> SimTime >> BfsStartTime >> AggStartTime;
    cin >> DisStartTime >> DataTransTime;

    

    node::node_generator::generate("IoT_sink",0);
    for (unsigned int id = 1; id < Nodes; id ++){
        node::node_generator::generate("IoT_device",id);    
    }

    for (unsigned int id = 0; id < Nodes; id ++){
        cin >> NodeId >> DataSize;
        node::id_to_node(NodeId)->add_data_size(DataSize);
        node::id_to_node(NodeId)->set_packet_size(PacketSize);
    }
    

    for (unsigned int id = 0; id < Links; id ++){
        cin >> LinkId >> LinkEnd1 >> LinkEnd2;
        node::id_to_node(LinkEnd1)->add_phy_neighbor(LinkEnd2);
        node::id_to_node(LinkEnd2)->add_phy_neighbor(LinkEnd1);    
    }

    
    
    
    // node 0 broadcasts a msg with counter 0 at time 100
    IoT_ctrl_packet_event(0, BfsStartTime);
    // 1st parameter: the source; the destination that want to broadcast a msg with counter 0 (i.e., match ID)
    // 2nd parameter: time (optional)
    // 3rd parameter: msg for debug information (optional)



    
    // node 4 sends a packet to node 0 at time 200 --> you need to implement routing tables for IoT_devicees
    for (unsigned int id = 1; id < Nodes; id ++){
        IoT_data_packet_event(id, 0, DataTransTime); 
    }

    // 1st parameter: the source node
    // 2nd parameter: the destination node (sink)
    // 3rd parameter: time (optional)
    // 4th parameter: msg for debug (optional)
    
    for (unsigned int id = 1; id < Nodes; id ++){
        AGG_ctrl_packet_event(id, 0, AggStartTime);
    }

    
    // 1st parameter: the source node
    // 2nd parameter: the destination node (sink)
    // 3rd parameter: time (optional)
    // 4th parameter: msg (for storing nb list)
    
    DIS_ctrl_packet_event(0, DisStartTime);
    // 1st parameter: the source node (sink)
    // 2nd parameter: the destination node
    // 3rd parameter: parent 
    // 4th parameter: time (optional)
    // 5th parameter: msg for debug (optional)

    // start simulation!!
    event::start_simulate(SimTime);
    //event::flush_events() ;
    // cout << packet::getLivePacketNum() << endl;


    /*
    for (unsigned int id = 1; id < Nodes; id++){
        cout << id << " ";
        dynamic_cast<IoT_device *>(node::id_to_node(id))->DisplayChildren();
        //cout<<dynamic_cast<IoT_device *>(node::id_to_node(id))->GetParent();
        cout << "\n";
    }
    */
    
    
    cout << "0 0\n";
    for (unsigned int id = 1; id < Nodes; id++){
        cout << id << " ";
        cout<<dynamic_cast<IoT_device *>(node::id_to_node(id))->GetParent();
        cout << "\n";
    }

    return 0;
}
