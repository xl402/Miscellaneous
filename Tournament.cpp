//N people tournament matching demonstration

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <direct.h>
using namespace std;

//Initialise a class of players, redundant for the assignment but can be used in future to store information about the player
class Player {
public:
	string player_id;
};

//Used for tournaments, structure of player against player
struct PlayerVSPlayer {
public:
	Player player1;
	Player player2;
};

/*
This function handles Questions 1 and 3, input: vector of teams(vector of players), output: order of tournaments
provided in vectour of rounds, each round is a vector of PlayerVSPlayers
The whole algorithm is based upon the Round-Robin Tournament algorithm found on Wikipedia
https://en.wikipedia.org/wiki/Round-robin_tournament
*/
vector<vector<PlayerVSPlayer>> tournamentOrder(vector<Player> teams) {

	//Initialise results, one for storing the first half of tournament, the other for second half
	//since each player plays against the other player twice
	vector<vector<PlayerVSPlayer>> results;
	vector<vector<PlayerVSPlayer>> results2;
	//Bonus part, handles odd number of players (add a "bye" player to the teams)
	if (teams.size() & 1 != 0) {
			teams.push_back({ "Bye" });
		}
		//Initialising iterators
		int size = teams.size();
		int half_iterator = size / 2;

		//Copy teams excluding the first member into another vector
		vector<Player> working_teams;
		working_teams.assign(teams.begin() + 1, teams.end());
		
		
		for (int i=0; i < size-1; i++) {
			//round1 stores player against player, round2 store them in reverse order for the second half
			//of the tournament
			vector<PlayerVSPlayer> round1;
			vector<PlayerVSPlayer> round2;
			//Matches the first player of the teams with the vector of the working teams
			PlayerVSPlayer p{ teams[0], working_teams[0] };
			round1.push_back(p);
			p = { working_teams[0], teams[0] };
			round2.push_back(p);
			//Matching the rest of the players within the working teams (1st paried with last, 2nd with 2nd last...)
			for (int j = 1; j < half_iterator; j++) {
				PlayerVSPlayer p{ working_teams[j], working_teams[working_teams.size() - j] };
				round1.push_back(p);
				//store them in reverse order again for the second half of the tournament
				p = { working_teams[working_teams.size() - j],  working_teams[j] };
				round2.push_back(p);
			}
			results.push_back(round1);
			results2.push_back(round2);
			//Following the Round-Robin algorithm, rotate the working teams to the left by a member
			rotate(working_teams.rbegin(), working_teams.rend() - 1, working_teams.rend());
		}
		//Add the second half of the tournament to the first half
		for (int j = 0; j < results2.size(); j++) {
			results.push_back(results2[j]);
		}
	return results;
}

/*
Function which writes each round of the tournament to a folder named Tournament_results, this function
needs improvement----maybe check whether the folder is already there? If so then name another folder?
setting console_out to true will print out the tournament orders
*/
void write_tournament_to_file(vector<vector<PlayerVSPlayer>> order_of_tournament, bool console_out=false) {
	mkdir("./Tournament_results");
	for (int i = 0; i < order_of_tournament.size(); i++) {
		vector<PlayerVSPlayer> temp;
		temp = order_of_tournament[i];
		string name = "./Tournament_results/Round" + to_string(i + 1) + ".txt";
		ofstream outFile(name);
		for (int j = 0; j < temp.size(); j++) {
			if (console_out) { cout << temp[j].player1.player_id << " vs " << temp[j].player2.player_id << endl; }
			outFile << temp[j].player1.player_id << " vs " << temp[j].player2.player_id << "\n";
		}
		if (console_out) { cout << endl; }
	}
	cout << "All tournament rounds written to directory /Tournament_results" << endl;
}


int main()
{
	//Four the purpose of the exercise, initalise N number of players
	vector<vector<PlayerVSPlayer>> order_of_tournament;
	vector<Player> teams;
	int N = 19;
	for (int i = 1; i < N+1; i++) {
		Player someplayer{ to_string(i) };
		teams.push_back(someplayer);
	}

	//Return tournament order given the team
	order_of_tournament=tournamentOrder(teams);
	//Write the order to file, second argument true for console output
	write_tournament_to_file(order_of_tournament,true);
	
    return 0;
}
