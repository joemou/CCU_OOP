        int visa = p3->getHeader()->getVisa();

        //cout << "visa value: " << visa << endl;


        //cout << endl <<getNodeID() << " " << p->getHeader()->getPreID()<<"(packe counte "<< l3->getCounter() <<")(on iot"<<counter<<")"<<endl;



        if(l3->getCounter()<counter){
            hi = false;
            //cout << endl << "counter big than prev\n\n\n\n\n\n\n\n\n\n\n"<<getNodeID() << " " << p->getHeader()->getPreID() << endl;
        }
        if(p->getHeader()->getPreID()<GetParent()&&l3->getCounter()==counter){
            hi = false;
            //cout << "\n\n\n\n" << "id big than store Node:"<<getNodeID() << " packet from who " << p->getHeader()->getPreID() <<" orgpa "<<GetParent()<< endl;
        }
        
        if(hi && FindConnect(p->getHeader()->getPreID())){
            //cout << "again fuck\n";
            hi = true;
        }
        if(l3->getCounter()>counter){
            hi = true;
        }

        if(visa==1 && getNodeID() == p3->getHeader()->getParent()){
            AddChild(p3->getHeader()->getPreID());

            hi = true;
        }
        if(visa==2 && getNodeID() == p3->getHeader()->getParent()){
            cout<<"===" << getNodeID()<<"add"<<p3->getHeader()->getPreID();
            AddChild(p3->getHeader()->getPreID());  
            hi = true;
        }
        if(visa==2 && getNodeID() == p3->getHeader()->getOldparent()){
            cout<<"===" << getNodeID()<<"kill"<<p3->getHeader()->getPreID();
            DeleChild(p3->getHeader()->getPreID());
            hi = true;
        }
        //cout << "{Now node}" << getNodeID() << ":\n";
        //DisplayChildren();

        cout << "\n%%" << p3->getHeader()->getVisa()<<" "<<p3->getHeader()->getParent()<<" "<<p3->getHeader()->getOldparent()<<" "<<p3->getHeader()->getPreID()<<"\n";
        for (unsigned int id = 1; id < 8; id++)
        {
            cout << id << " ";
            dynamic_cast<IoT_device *>(node::id_to_node(id))->DisplayChildren();
            // cout<<dynamic_cast<IoT_device *>(node::id_to_node(id))->GetParent();
            cout << "\n";
            }


        // setVisa 0 is normal 1 is ack 2 is notify child bye
        if(!hi){
            //cout << "\nB\n";
            counter = l3->getCounter();

            p3->getHeader()->setParent(p->getHeader()->getPreID());
            
            if(GetParent()==99999){
                p3->getHeader()->setVisa ( 1 );
            }
            else{
                p3->getHeader()->setVisa ( 2 );
                p3->getHeader()->setOldparent(GetParent());
            }

        

            SetParent(p3->getHeader()->getPreID());//set new parent

            const map<unsigned int, bool> &nblist = getPhyNeighbors();
            for (map<unsigned int, bool>::const_iterator it = nblist.begin(); it != nblist.end(); it++)
            {
                if(it->second){
                    AddConnect(it->first);
                }
            }

            //Org Broadcast
            p3->getHeader()->setPreID ( getNodeID() );
            p3->getHeader()->setNexID ( BROADCAST_ID );
            p3->getHeader()->setDstID ( BROADCAST_ID );
            l3->increase();
            hi = true;
            send_handler(p3);

        } 