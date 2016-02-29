#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h> 

using namespace std;                                                                                    

namespace rws2016_moliveira
{

class Player
{
    public:

    //Constructor with the same name as the class
    Player(string name) { this->name = name;setTeamName();}
    Player(string name, string team) { this->name = name; setTeamName(team);}
    Player(string name, int team_index) { this->name = name; setTeamName(team_index);}

    //Set team name, if given a correct team name (accessor)
    int setTeamName(string team="red")
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

    int setTeamName(int team_index)
    {
        if (team_index==0) setTeamName("red");
        else if (team_index==1) setTeamName("green");
        else if (team_index==2) setTeamName("blue");
        else
        {
            cout << "cannot set team name to index " << team_index << endl;
            return 0;
        }
    }
    //Gets team name (accessor)
    string getTeam(void) {return team;}

    string name; //A public atribute

    private:
    string team;
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

}//end of namespace


int main()
{
    //Creating an instance of class Player
    vector<string> players;
    players.push_back("moliveira");
    players.push_back("vsantos");
    players.push_back("pdias");

    rws2016_moliveira::Team team("green", players);
    team.printTeamInfo();
} 


