#include <iostream>
#include <fstream>
#include <vector>

#include <ctime>
#include <stdlib.h>

#include <cmath>

#include "Passenger.h"

//0: Empty seat, 1: Empty Aisle, 2: filled aisle, 3: filled seat, 4: waiting passenger
std::vector<std::vector<int> > createPlane()
{
	std::vector<std::vector<int> > plane;

	for (int i = 0; i < 40; ++i)
		plane.push_back(std::vector<int>(7));

	for (int i = 0; i < plane.size(); ++i)
	{
		for (int j = 0; j < plane[i].size(); ++j)
			plane[i][j] = ((j == 3) ? 1 : 0);
	}

	return plane;
}

//1: vertical, 2: horizontal
void printPlane(std::vector<std::vector<int> > plane, int mode = 0)
{
	if (mode == 0)
	{
		for (int i = 0; i < plane.size(); ++i)
		{
			std::cout << i << ": \t";
			for (int j = 0; j < plane[i].size(); ++j)
			{
				if (plane[i][j] == 1)
					std::cout << '\t';
				else
					std::cout << plane[i][j] << ' ';
			}

			std::cout << std::endl;
		}
	}
	else if (mode == 1)
	{
		for (int i = 0; i < plane[i].size(); ++i)
		{
			for (int j = 0; j < plane.size(); ++j)
			{
				std::cout << plane[j][i] << " ";
			}
			std::cout << std::endl;
		}
	}
}

//1: colum, 2: row, 3: boarding position, 4: ..., 5: Seat number
void printPassengers(std::vector<passenger> pass, int mode = 0)
{
	for (int i = 0; i < 40; ++i)
	{
		std::cout << i << ": " << '\t';
		for (int j = 0; j < 6; ++j)
		{
			switch (mode){
			case 1:
				std::cout << pass[j * 40 + i].getSeat() % 7 << '\t';
				break;
			case 2:
				std::cout << pass[j * 40 + i].getSeat() / 7 << '\t';
				break;
			case 3:
				for (int k = 0; k < pass.size(); ++k)
				{
					if (pass[k].getSeat() == i * 7 + j + ((j <= 2) ? 0 : 1))
					{
						std::cout << k + 1 << '\t';
					}
				}
				break;
			case 4:
				std::cout << pass[i * 6 + j].getSeat() << " ";
				break;
			default:
				std::cout << pass[j * 40 + i].getSeat()<< '\t';
			}
			//if (j == 2)
			//	++j;
		}
		std::cout << std::endl;
	}
}

bool checkDuplicate(std::vector<passenger> passengers)
{
	std::vector<int> sn;
	for (int k = 0; k < passengers.size(); ++k)
	{
		sn.push_back(passengers[k].getSeat());
	}
	for (int k = 0; k < sn.size(); ++k)
	{
		for (int l = k + 1; l < sn.size(); ++l)
		{
			if (sn[k] == sn[l])
			{
				std::cout << sn[k] << std::endl;
				printPassengers(passengers, 0);
				return false;
			}
		}
	}

	return true;
}

std::vector<passenger> createPassengers(int n, int luggage = 0)
{
	std::vector<passenger> pass;
	int cnt = 0;
	for (int i = 0; i < n; ++i)
	{
		pass.push_back(passenger(((luggage == -1) ? ((float)rand() / RAND_MAX * 2): luggage), i + cnt));
		if ((i + cnt) % 7 == 2)
			++cnt;
	}

	//randomize passenger distr.
	std::vector<passenger> pass_rand;

	std::vector<int> r(pass.size());
	for (int i = 0; i < pass.size(); ++i)
	{
		r[i] = i;
	}
	for (int i = 0; i < pass.size(); ++i)
	{
		int vrand = (float)rand() / RAND_MAX * (pass.size() - 1);
		while (r[vrand] == -1)
		{
			vrand += 1;
			if (vrand == pass.size())
				vrand = 0;
		}
		r[vrand] = -1;

		pass_rand.push_back(pass[vrand]);
	}

	//return pass;
	return pass_rand;
}

// 0: Outside in, 1: outside in with groups, 2 rear to front, 3 Steffen 1, 4 FFA, 5 Assigning seats at boarding
std::vector<passenger> sortPassengers(std::vector<passenger> pass, std::vector<std::vector<int> > plane, int method, std::vector<int> para = std::vector<int>(0))
{
	std::vector<passenger> sortPass;
	//Sorting algorithm according to boarding method.
	if (method == 0)
	{
		//sorting passengers according to window/middle/aisle; no grouping
		std::vector<int> num(7);
		num[0] = 0;
		num[1] = 6;
		num[2] = 1;
		num[3] = 3; // not used anyways
		num[4] = 5;
		num[5] = 2;
		num[6] = 4;

		for (int i = 0; i < 7; ++i)
		{
			for (int j = 0; j < pass.size(); ++j)
			{
				if (pass[j].getSeat() % 7 == num[i])
					sortPass.push_back(pass[j]);
			}
			if (i == 2)
				++i;
		}

	}
	else if (method == 1)
	{
		//sorting passengers according to window/middle/aisle
		std::vector<int> num(7);
		num[0] = 0;
		num[1] = 6;
		num[2] = 1;
		num[3] = 3; // not used anyways
		num[4] = 5;
		num[5] = 2;
		num[6] = 4;

		for (int i = 0; i < 7; ++i)
		{
			for (int j = 0; j < pass.size(); ++j)
			{
				if (pass[j].getSeat() % 7 == num[i])
					sortPass.push_back(pass[j]);
			}
			if (i == 2)
				++i;
		}

		//making the groups
		int grps = para[0];
		int rows = round(sortPass.size() / 6.0);
		float grpSize = float(rows) / grps;

		for (int i = 0; i < 6; ++i)
		{
			for (int l = 0; l < grps - 1; ++l)
			{
				int rGrpSize = round(grpSize*(l + 1));

				for (int j = round(grpSize * l) + i*rows; j < rGrpSize + i * rows; ++j)
				{
					if (!(sortPass[j].getSeat() / 7 < rGrpSize))
					{
						for (int k = rGrpSize + i*rows; k < rows + i*rows; ++k)
						{
							if (sortPass[k].getSeat() / 7 < rGrpSize)
							{
								passenger t = sortPass[j];
								sortPass[j] = sortPass[k];
								sortPass[k] = t;
							}
						}
					}
				}
			}
		}

		std::vector<passenger> temp = sortPass;
		for (int j = 0; j < 6; ++j)
		{
			for (int i = 0; i < sortPass.size() / 6; ++i)
			{
				temp[i + sortPass.size() / 6 * j] = sortPass[sortPass.size() / 6 + sortPass.size() / 6 * j - 1 - i];
			}
		}
		sortPass = temp;
	}
	else if (method == 2)
	{
		//existing method: rear to front
		sortPass = pass;

		int grps = para[0];
		float grpSize = float(sortPass.size()) / grps;

		for (int i = 0; i < grps-1; ++i)
		{
			for (int j = round(grpSize*i); j <= round(grpSize*(i + 1)); ++j)
			{
				if (sortPass[j].getSeat() - sortPass[j].getSeat()/7 > round((i + 1)*grpSize))
				{
					for (int k = round(grpSize*(i + 1)); k < sortPass.size(); ++k)
					{
						if (sortPass[k].getSeat() - sortPass[k].getSeat() / 7 <= round((i + 1)*grpSize))
						{
							passenger t = sortPass[j];
							sortPass[j] = sortPass[k];
							sortPass[k] = t;
						}
					}
				}
			}
		}

		std::vector<passenger> temp = sortPass;
		for (int i = 0; i < sortPass.size(); ++i)
		{
			temp[i] = sortPass[sortPass.size()-1-i];
		}
		sortPass = temp;
		
	}
	else if (method == 3)
	{
		//STEFFEN
		std::vector<int> num(7);
		num[0] = 0;
		num[1] = 6;
		num[2] = 1;
		num[3] = 3; // not used anyways
		num[4] = 5;
		num[5] = 2;
		num[6] = 4;

		for (int i = 0; i < 7; ++i)
		{
			for (int j = 0; j < pass.size(); ++j)
			{
				if (pass[j].getSeat() % 7 == num[i])
					sortPass.push_back(pass[j]);
			}
			if (i == 2)
				++i;
		}

		//making the groups
		int grps = 40;
		int rows = round(sortPass.size() / 6.0);
		float grpSize = float(rows) / grps;

		for (int i = 0; i < 6; ++i)
		{
			for (int l = 0; l < grps - 1; ++l)
			{
				int rGrpSize = round(grpSize*(l + 1));

				for (int j = round(grpSize * l) + i*rows; j < rGrpSize + i * rows; ++j)
				{
					if (!(sortPass[j].getSeat() / 7 < rGrpSize))
					{
						for (int k = rGrpSize + i*rows; k < rows + i*rows; ++k)
						{
							if (sortPass[k].getSeat() / 7 < rGrpSize)
							{
								passenger t = sortPass[j];
								sortPass[j] = sortPass[k];
								sortPass[k] = t;
							}
						}
					}
				}
			}
		}

		std::vector<passenger> temp = sortPass;
		for (int j = 0; j < 6; ++j)
		{
			for (int i = 0; i < sortPass.size() / 6; ++i)
			{
				temp[i + sortPass.size() / 6 * j] = sortPass[sortPass.size() / 6 + sortPass.size() / 6 * j - 1 - i];
			}
		}
		sortPass = temp;
	}
	else if (method == 4)
	{
		//Free for all: making small "groups" of families/friends
		//randomly choosing passengers and adding their neighbour as the next boarding person
		sortPass = pass;

		int num = para[0];
		for (int i = 0; i < num; ++i)
		{
			int rPass = 0;

			do
			{
				rPass = (float)rand() / RAND_MAX * (sortPass.size() - 1);
			} while (sortPass[rPass].getSeat() % 7 != 1 && sortPass[rPass].getSeat() % 7 != 4);

			int pos = sortPass[rPass].getSeat();

			for (int j = 0; j < sortPass.size(); ++j)
			{
				int nPos = sortPass[j].getSeat();

				if ((nPos == pos - 1 || nPos == pos + 1) && nPos / 7 == pos / 7)
				{
					if (rPass > 0 && sortPass[rPass - 1].getSeat() != pos - 1 && sortPass[rPass - 1].getSeat() != pos + 1)
					{
						passenger temp = sortPass[j];
						sortPass[j] = sortPass[rPass - 1];
						sortPass[rPass - 1] = temp;
					}
					else if (rPass + 1 < sortPass.size())
					{
						passenger temp = sortPass[j];
						sortPass[j] = sortPass[rPass + 1];
						sortPass[rPass + 1] = temp;
					}

					if (round((float)rand() / RAND_MAX))
						break;
				}
			}
		}
	}
	else if (method == 5)
	{
		sortPass = pass;

		int rows = round(sortPass.size() / 6.0);
		std::vector<int> num(7);
		num[0] = 0;
		num[1] = 6;
		num[2] = 1;
		num[3] = 5;
		num[4] = 2;
		num[5] = 4;

		//proposed method 2: assigning the passengers a seat when they board
		for (int i = 0; i < pass.size(); ++i)
		{
			//calculate seating position for each passenger
			int seat = 40 + sortPass.size() - num[i/rows] - (i%40)*7 - 1;
			sortPass[i].assignSeat(seat);
		}
	}

	return (sortPass.size() == 0) ? pass : sortPass;
}

bool checkSeating(std::vector<passenger> pass)
{
	for (int i = 0; i < pass.size(); ++i)
	{
		if (!pass[i].getSeatingStatus())
			return false;
	}

	return true;
}

int calcSitTime(const std::vector<int> &row, int pos)
{
	int col = pos % 7;
	int cnt = 0;
	int fct = (col > 3) ? 1 : -1;

	for (int i = 3; (fct == 1 && i <= col) || (fct == -1 && i >= col); i += fct)
	{
		if (row[i] == 3)
			++cnt;
	}

	return cnt;
}

int board(std::vector<std::vector<int> > &plane, std::vector<passenger> &pass)
{
	int timer = 0;
	int cnt = 0;

	//std::ofstream file("brd.txt");

	while (!checkSeating(pass))
	{
		for (int i = 0; i < pass.size(); ++i)
		{
			if (pass[i].getPos() == -1 && plane[0][3] == 1)
			{
				pass[i].changePos(3);
				plane[0][3] = 2;
			}

			else if (!pass[i].getSeatingStatus() && pass[i].getPos() != -1)
			{
				int pos = pass[i].getPos();

				if (pass[i].getSeat() / 7 == pos / 7)
				{
					if (pass[i].storeLuggage())
					{
						//Calculate time to sit down if it hasn't been done before (3 time units per passenger already sitting)
						if (pass[i].getSitTime() == -1)
						{
							int tm = calcSitTime(plane[pos / 7], pass[i].getSeat()) * 3;
							pass[i].setSitTime(tm);
						}

						//Sitting down (takes the time calculated above)
						if (pass[i].sitDown())
						{
							plane[pos / 7][pos % 7] = 1;
							plane[pass[i].getSeat() / 7][pass[i].getSeat() % 7] = 3;

							pass[i].changeSeatingStatus();
						}
						else if (plane[pos / 7][pos % 7] == 2)
						{
							plane[pos / 7][pos % 7] = 4;
						}
					}
				}

				//Search aisle
				else
				{
					int row = pos / 7;
					if (row < plane.size())
					{
						if (plane[row + 1][pos % 7] == 1)
						{
							plane[row][pos % 7] = 1;
							plane[row + 1][pos % 7] = 2;

							pass[i].changePos((row + 1) * 7 + pos % 7);
						}
					}
				}
			}
		}

		timer += 1;

		//printPlane(plane, 0);

		/*for (int i = 0; i < plane.size(); ++i)
		{
			file << i << ": \t";
			for (int j = 0; j < plane[i].size(); ++j)
			{
				file << plane[i][j] << ' ';
			}

			file << std::endl;
		}*/

		/*int c = 0;
		for (int i = 0; i < pass.size(); ++i)
			if (pass[i].getSeatingStatus())
				std::cout << pass[i].getSeat()<<" ";

		std::cout << std::endl;*/
		//std::cout << c << std::endl;
	}

	return timer;
}

void runSimulation()
{
	std::vector<std::vector<int>> paras(6, std::vector<int>(2));
	std::vector<int> map(6);
	map[0] = 3;
	map[1] = 4;
	map[2] = 2;
	map[3] = 0;
	map[4] = 1;
	map[5] = 5;

	//first method: Steffen
	paras[0][0] = 0;
	paras[0][1] = 0;

	//random with assigned seats; worst: -, best -
	paras[1][0] = 70;
	paras[1][1] = 0;

	// rear to front; worst: 3, best 10
	paras[2][0] = 3;
	paras[2][1] = 10;

	// outside in; worst: -, best -
	paras[3][0] = 0;
	paras[3][1] = 0;

	// outside in with groups; worst: 3, best 10
	paras[4][0] = 3;
	paras[4][1] = 10;

	// assignment at boarding; worst: 50, best 0
	paras[5][0] = 50;
	paras[5][1] = 0;

	int reps = 1000;

	std::ofstream file("analysis_all.txt");
	for (int i = 0; i < paras.size(); ++i)
	{
		std::cout << i << " ";

		file << i << std::endl;

		for (int k = -1; k<3; ++k)
		{
			std::cout << k << " ";

			for (int l = 0; l < 2; l++)
			{
				int tm = 0;

				for (int j = 1; j <= reps; ++j)
				{
					int seats = (7 - 1) * 40;
					std::vector<std::vector<int> > plane = createPlane();

					std::vector<passenger> passengers = createPassengers(seats, k);

					std::vector<int> para(1);
					para[0] = paras[i][l];
					passengers = sortPassengers(passengers, plane, map[i], para);

					tm += board(plane, passengers);
				}

				file << tm / reps << '\t';
			}
		}

		std::cout << std::endl;
		file << std::endl;
	}
}

int main()
{
	srand(time(NULL));

	float avg = 0;
	int reps = 100;

	//std::ofstream file("rand_grps.txt");

	/*for (int i = 1; i <= 100; ++i)
	{
		std::cout << "Run: "<<i<<std::endl;
		file << i << '\t';

		for(int k=-1; k<3; ++k)
		{
			int tm = 0;

			for (int j = 1; j <= reps; ++j)
			{
				int seats = (7 - 1) * 40;
				std::vector<std::vector<int> > plane = createPlane();

				std::vector<passenger> passengers = createPassengers(seats, k);

				std::vector<int> para(1);
				para[0] = i;
				passengers = sortPassengers(passengers, plane, 5, para);

				tm += board(plane, passengers);
			}
			file << tm / reps << '\t';
		}

		file << std::endl;
	}*/

	runSimulation();

	return 0;
}
