#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h> 

using namespace std;

namespace rws2016_torrao
{
    class Player
    {
        public:

            //Constructor with the same name as the class
            Player(string name) {this->name = name;}
            Player(string name, string team) {this->name = name;setTeamName(team);}

            int setTeamName(int team_index = 0 /*default value*/)
            {
                switch (team_index)
                {
                    case 0: 
                        return setTeamName("red"); break;
                    case 1: 
                        return setTeamName("green"); break;
                    case 2: 
                        return setTeamName("blue");  break;
                    default: 
                        cout << "wrong team index given. Cannot set team" << endl; break;
                }
            }

            //Set team name, if given a correct team name (accessor)
            int setTeamName(string team)
            {
                if (team=="red" || team=="green" || team=="blue")
                {
                    this->team = team;
                    return 1;
                }
                else
                {
                    cout << "cannot set team name to " << team << endl;
                    return 0;
                }
            }

            //Gets team name (accessor)
            string getTeamName(void) {return team;}

           /**
             * @brief Gets the pose (calls updatePose first)
             *
             * @return the transform
             */
            tf::Transform getPose(void)
            {
                ros::Duration(0.1).sleep(); //To allow the listener to hear messages
                tf::StampedTransform st; //The pose of the player
                try{
                    listener.lookupTransform("/map", name, ros::Time(0), st);
                }
                catch (tf::TransformException ex){
                    ROS_ERROR("%s",ex.what());
                    ros::Duration(1.0).sleep();
                }

                tf::Transform t;
                t.setOrigin(st.getOrigin());
                t.setRotation(st.getRotation());
                return t;
            }

            string name; //A public atribute

        private:
            string team;
            tf::TransformListener listener; //gets transforms from the system
    };



class Team
{
    public: 

    //Team constructor
    Team(string team, vector<string>& player_names)
    {
       name = team; 

       //Cycle all player names, and create a class player for each
       for (size_t i=0; i < player_names.size(); ++i)
       {
           //Why? Copy constructable ...
           boost::shared_ptr<Player> p(new Player(player_names[i]));
           p->setTeamName(name);
           players.push_back(p);
       }

    }

    void printTeamInfo(void)
    {
        cout << "Team " << name << " has the following players:" << endl;

        for (size_t i=0; i < players.size(); ++i)
            cout << players[i]->name << endl;
    }

    string name;
    vector<boost::shared_ptr<Player> > players;
};  

} //end of namespace rws2016_torrao

int main(int argc, char** argv)
{
    //initialize ROS stuff
    ros::init(argc, argv, "player_torrao_node");
    ros::NodeHandle node;

    //Creating an instance of class MyPlayer
    rws2016_torrao::Player my_player("torrao", "blue");

    //Infinite loop
    ros::Rate loop_rate(10);
    while (ros::ok())
    {
        //Test the get pose method                                                                                     
        tf::Transform t = my_player.getPose();
        cout << "x = " << t.getOrigin().x() << " y = " << t.getOrigin().y() << endl;

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}


