# brd
Particle-like simulation of the boarding process of an airplane.

## Introduction
Have you ever been annoyed by the painfully long queue, while boarding on a plane? That moment, in front of the forming boarding line at the gate, when you are internally torn by the dilemma between joining the line before it gets too long, or waiting long enough, sitting on your bench, till the line gets shorter? Or, once in the airplane, that frustration of waiting for this one passenger in front of you, who is blocking the whole line, loading his luggage up in the cabin?

The good news is, you are not alone. Shortening the boarding time on an airplane has been the holy grail of airline companies since a long while. Many different strategies have already been adopted by airline companies to this end, such as controlling the boarding order of passengers according to their seats by assigned groups. But none of them have proved themselves satisfactory.

In this project, we will implement a new strategy with a brand new approach, based on Jason Steffen's work, Optimal boarding method for airline passengers<ref>http://home.fnal.gov/~jsteffen/airplanes.html</ref>. A strategy, that will not only reduce the cost for airline companies, but that will also make you happy.

### Motivation
A plane on the ground, doesn't bring money. We pay for the trip, no matter how long the plane stays on ground before taking off. It is estimated that every minutes a plane spends on ground, represents a loss of 40 USD for the company, according to the Airlines of America organisation <ref>http://www.airlines.org/Pages/Annual-and-Per-Minute-Cost-of-Delays-to-U.S.-Airlines.aspx</ref>. In that sence, reducing the time a plane needs on ground, is an interesting issue for airline companies, since it allows the plane to make more trips per time unit. A plane needs to spend some time on ground for various reasons, such as reloading food, fuel, cleaning, unloading/loading luggages, and most importantly, boarding of passengers. Even though some of these tasks can be realized parallely, studies shows that the boarding time of passengers represent around 60% (up to 40 minutes) of the total time volume a plane spends on ground. At the same time, it is the most compressible task, compared to the others. Reducing this boarding time by a factor ofeven 30% would save millions of EUR, given the large amount of flights and planes an average company possesses.

### Idea of the solution
We will implement a simple simulation of the boarding process, and compare the time cost of different strategies. The idea is to get a strategy close to the time-optimal solution proposed by Steffen, but slightly modified to take account of the client satisfaction.

=Approach=

### Jason Steffen's Work (1) 
Jason Steffen, in his work, found through a Monte-Carlo simulation the theoretical optimal boarding strategy. Let us briefly discuss it. 

Intuitively, we might think that the easy way to optimize the boarding process would be to board the passengers from the rear to the front. That way, it seems like no one would block the way of any passengers in the aisle. It turns out that, letting the passengers board exactly from the last row till the front row, is one of the worst boarding strategy in terms of time. The reason for this is as follows: the boarding speed actually depends on the number of people who are loading their luggage ''simultaneoustly'', at a given time. In fact, by letting all the passengers in the back row board and so on, we can easily see that at any given point, there are actually one, at most two passengers who can simultaneously load their luggage. If two people of the same row follow each other, the one behind has to indeed wait for the one in front to load his luggage and sit, before he can proceed the same. Hence, if the average loading time of luggage + sitting time is ''M'', the whole boarding process of ''N'' passengers could take a time up to ''N*M''. Steffen actually showed that this back-to-front method, though adopted by some companies, is actually worse than a simple free-for-all random model where passengers board without any strategy. These companies are literally investing more to lose more time.

Steffen's idea for his optimal strategy is based on this observation that, in order to minimize the boarding time, one has to maximize the number of passengers simultaneously loading their luggage. If, for instance, at any given time, 10 passengers are loading their luggage in the aisle, then the total boarding time would be around N*M/10 with the previous notations. Intuitively, Steffen's ideal strategy focuses on "parallelizing"  the boarding process, instead of the "serial" approach used by most companies.

#### Details of the strategy
Here is the technical detail of this optimal strategy. Let us consider an airplane with one aisle and N rows of seats. The optimal boarding strategy consists in letting one passenger from the last row (row N) board first, followed by a passenger of the row N-1, then one from row N-2, and so on, till we reach the front row. After each row has been filled by one passenger, we start the process again. This choice of boarding order leaves all passengers in the aisle with enough space to load their luggage, and therefore allows on around N passengers loading their luggage simultaneously. Moreover, instead of randomly choosing a passenger from a given row, we fill a given row from the window side to the aisle side. That way, there will not be any interference between passengers of the same row, avoiding that one has to stand up to let another one seat. This is the optimal strategy implemented by Steffen.

#### Performance (Needed Time) 
The sorting algortihm is esentially the same as the on used for our block-method, just with a hardcoded amount of groups (the same as the number of rows).

We first sort the people according to their seat (window/middle/asile). Then, we go through every seat-column, compare their row, and switch them in case the necessary requirement is fulfilled.
<source lang="cpp">
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
</source>

[[File:steffen_optimal.gif|500px|center|thumb|Steffens first method shows the optimal way to board. Notice that there isn't a orange passenger ever. (Depending on the browser, you have to click the thumbnail to see the animation)]]

#### Customer Satisfaction 
The theoretical optimality of the strategy is obvious and the gain in time is impressive. The drawback of this method is that it is in practice complicated to implement. It requires the airline company to completely sort out the passengers according to their seat, (not by blocks, but by individual seats!), and obviously the customers are not going to want to go through a process which is that "complicated", just to save a few minutes for the airline.

### Jason Steffen's Work (2): Free-For-All
We are going to implement the second of Steffen's publications as well.

He modeled the Free-For-All boarding by assigning a potential to the seats according to their attractivity and the passengers' preferences (resulting in high potentials for window/aisle seats, and a relatively low potential for the middle ones). He then let the people board with different parameters (e.g. laid-backness), and came to the result that even the random boarding is better than the currently used model.

However, while being one of the fastest boarding methods, it isn't favorable in terms of costumer satisfaction. <ref>http://flightclub.jalopnik.com/mythbusters-proves-most-airlines-board-planes-all-wrong-1636981904</ref>

#### With assigned seats 

##### Implementation
We already get a randomized vector containing the passengers, and we don't even have to sort it this time. We might want to consider groups of people (friends, family), which are sitting together, and are very likely to board together as well.

<source lang="cpp">
//Free for all: making small "groups" of families/friends
//randomly choosing passengers and adding their neighbour as the next boarding person
sortPass = pass;

int num = para[0];
for (int i = 0; i < num; ++i)
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
</source>

In this piece of code, we take the completely random vector with the passengers, and create as many groups consisting of up to three people as we want to. Statistically, about half of the groups consist of only two people, the other half is made up of three passengers.

#####Performance
The number of pairs/groups of passengers does not impact the needed boarding time greatly, as seen in the graph below. However, there is an observation to be made. We can see an upwards slope in the "1 bag" and in the "2 bags" curve. The reasoning behind this is that two/three people which are boarding together store their respective luggage right after each other, making all the other passengers currently standing in the aisle wait. This effect takes a bigger impact towards the end, where we have 100 groups/triplets.

[[File:rand_w_grps.PNG|500px|center|thumb|average boarding time from 100 simulations for 100 cases with 4 different parameters]]

That effect can be seen in the curve for the simulation with one piece of luggage as well, but it isn't nearly as strong. Having a look at the raw data helps a lot for this case.

The "no luggage" case is pretty much unaffected, one might even say that the boarding time goes down here when the number of groups goes up. An explanation for this might be the missing storing time of the luggage, so the effect that fewer people have to wait until someone sitting in their way shows itself here.

#### True Free-For-All 
See above; The method is going to be similar to Steffen's.
###Existing Models
Most airlines currently use the same model. They divide the airplane in several sections (depending on the size of the airplane up to 6), and call for them to board at different times. That way, the plane is filled up from the back. The reasoning behind this strategy is that the people which board first don't block the aisle for the later passengers. However, this is not the best possible approach. Neither time-wise, nor does it maximize the customer satisfaction.

In detail there are six different known models: 1) Outside-In, 2) Random, 3) Rear to Front, 4) Reverse Pyramid, 5) Rotating Zone, 6) Zone/Block Style 
<ref>http://www.seatguru.com/articles/boarding_procedures.php</ref>

####Outside-In
There is only one Airline which is practicing this kind of model. The boarding procedure is also called WilMA (Window, Middle, Aisle). The first which may go on board are frequent elite flyer or other premium flyer. After the "elite-flyers" the boarding starts form Outside to Inside, from Window to Aisle.

<center>[[File:Boarding_WilMA.gif]]</center>

#####Implementation
<source lang="cpp">
//sorting passengers according to window/middle/aisle; no grouping, therefore not much faster than randomized
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
</source>

[[File:outside_in.gif|500px|center|thumb|Outside in without grouping. (Depending on the browser, you have to click the thumbnail to see the animation)]]

#####Performance
The performance of this method isn't terrible, although it's far from optimal. The process is already partly paralellized, but it often occurs that the passengers block each other when someone has to store his luggage. People don't have to stand up again to let other passengers get to their seat, so one of the time-consuming factors is eliminated.

<center>
 0 0 3 3 0 3 3 0 3 0 3 3 0 0 0 3 3 0 0 0 3 3 3 0 3 0 0 0 0 0 3 0 0 0 0 3 0 0 0 0
 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
 2 2 2 2 2 2 1 1 2 1 1 1 2 2 2 2 2 1 1 2 1 1 2 1 1 1 1 1 1 1 1 1 1 2 1 1 1 1 1 1
 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
</center>

How the plane looks after 50 timesteps is to be expected if we take the points from above into consideration.

####Random
The random boarding is not as random as the name suggests. Free-For-All would be a more accurate name (you can choose the seat in the airplane itself). Southwest Airlines implemented a sort of it, and are the only ones still using it.

Surprisingly, it's very efficient. After thinking about it for a bit, it makes sense though. The seats which are most likely to be taken first are either the window, or the aisle-seats. For minimizing the needed time, the window seats are the best ones to be taken first, while the aisle isn't as good.

For the simulation, implementation, performance and analysis/discussion see Steffens Work (2).

####Rear to front
The rear-to-front method is the most often used boarding method by airlines. First and business class passengers are boarded first. After boarding the "elites", the seats in the back of the plane are filled, followed by the middle section and at last the front area. This boarding method seems to be logical but it was proven as the slowest common used method of all. The problem is that the passengers all clump together, and get in each other’s way, slowing down the whole process.<ref> http://the-flying-carpet.com</ref>
<center>[[File:reartofront.png|661x661px]]</center><ref>http://www.seatguru.com/articles/boarding_procedures.php#Rear_to_Front</ref>

#####Implementation
<source lang="cpp">
//existing method
sortPass = pass;

int grps = para[0];
int rows = round(sortPass.size()/6.0);
float grpSize = float(sortPass.size()) / grps;

for (int i = 0; i < grps-1; ++i)
{
	for (int j = round(grpSize*i); j < round(grpSize*(i + 1)); ++j)
	{
		for (int k = round(grpSize*(i + 1)); k < sortPass.size(); ++k)
		{
			if (sortPass[j].getSeat() <= round((i+1)*grpSize))
			{
				break;
			}
			else if (sortPass[k].getSeat() <= round((i + 1)*grpSize))
			{
				passenger t = sortPass[j];
				sortPass[j] = sortPass[k];
				sortPass[k] = t;
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
</source>

[[File:rtf.gif|500px|center|thumb|Standard rear to front boarding method. (Depending on the browser, you have to click the thumbnail to see the animation)]]

#####Performance
The performance of this method is extremely interesting, as it's completely counter-intuitive. if we increase the number of groups (up to the number of seats), we observe a '''longer''' boarding phase.

[[File:rtf.PNG|500px|center|thumb|average boarding time from 100 simulations for 10 cases with 3 different parameters]]

The general differentiation makes sense: The simulation with the most luggage turns out to have the longest boarding time, as expected. The gaps are around 200 respectively 300 timeunits at 1 group, and go up to 450 resp. 650 at 10 groups.  The curves themselves are scaled versions of each other.

Where it gets counter-intuitive is the increasing boarding time for a higher amount of groups. If we take a look at the better sorted boarding process, it actually takes longer until all the passengers stored their luggage and are sitting in their seats. Taking a step back, and thinking about the boarding process, it makes sense:

* If the process is completely randomized, more people are storing their luggage at the same time because the passengers in the plane are more evenly distributed, and aren't waiting all for similar rows to store their bags.
* As the number of groups increases, the boarding gets more sorted, and therefore, more people are blocked at the same time by single passengers storing their luggage/waiting to being able to sit down.
* The slope of the simulation with two pieces of luggage reflects this observation.

####Reverse Pyramid 
This Model is very interesting, because no airline is still running it. This method has not been proven yet. Seat Guru says that "as of May 2009, US Airways no longer uses the reverse pyramid boarding style." The writer of this article on Seat Guru is amazed and says: "Although research shows the reverse pyramid style is an efficient boarding style, US Airways has opted to use the standard random seating order, giving preference to elite flyers and those who checked in online a rear-to-front boarding order."

Zones: 1. Top Elite, 2. Elite and first class, 3. Back windows, 4.Back middles and front windows, 5.Back aisles and front middles, 6.Front Aisles

<center>[[File:Boarding_Reverse_Pyramid.gif]]</center>

####Rotating Zone
Similar to the common rear-to-front method, the rotating zone-boarding style uses blocks. The difference is that the zones here "rotate" around in the plane. The first group fills the rear, the second is in a block in front, and then we go back to the rear again.

In theory, this boarding style doesn't really improve anything in comparison to the traditional rear-to-front method. We actually expect it to be worse, as the passengers from the second group block the aisle for the third group when storing their luggage.

We are not going to do a detailed analysis/simulation of this model, because you already see form the theory that it doesn't give you any advantages over the other methods.

#### Southwest Airline Method 
Southwest Airline is the largest low-cost-carriers in the world. They employ more than 46'000 and the airline is one of the most profitable ones in the world. Currently they are on the 4th place in this ranking. Southwest Airline is the example for a lot of other low-cost-carrier such as Ryanair or Easy-Jet. 

Prior to 2007, Southwest had a very usual method for boarding. They divided the passengers into three boarding groups: A, B and C. The boarding then followed by the letter. Group A boarded first and the passengers of group C boarded last. In 2007, Southwest Airline improved their boarding procedure. In addition to the boarding group A, B and C they added a number. Each group has individual position, for example "A 26". The number next to the letter is your position in line before boarding the plane ("column position"). The earlier a passenger books, the better boarding group and column position they get. Online check-in starts 24 hours before departure. Southwest has an open sitting-policy: once on board you can choose any seat you want. [[wikipedia:Southwest_Airlines|<nowiki/>]]

Southwest has an online video in which they present the boarding process: https://www.youtube.com/watch?v=X2B7EilM0pk

##### Performance (Needed Time)
As mentioned above, Southwest Airline is one of the most profitable airlines in the world '''[citation needed]'''. One of the reasons is that they save a lot of time on the ground'''[citation needed]'''. We already know that Southwest has a very fast boarding system'''[citation needed]'''. But they also have two jetways to bring the passengers as fast as possible from the aircraft'''[citation needed]'''.

##### Customer Satisfaction 
Southwest Airlines is consistently the airline with the lowest customer complaint rate in the industry'''[citation needed]'''. Maybe because there are three different types of boarding: 

# General boarding (explained above)
# Family boarding. If you are travelling with a child which is under four years old you are allowed to board the plane after group A and just before group B. 
# Line leader boarding. If you want to guarantee that you’ll be one of the first to board. Southwest calls it "Business select" or "Early Bird Check-in" [https://www.southwest.com/html/travel-experience/boarding-your-flight/]

//What is the reasoning behind the above? This whole piece needs to be rewritten badly. It is filled with random statements that have no evidence to back it up (read: lacking citations all around) and it is poorly formulated in general. I've tried moving it to the Talks page, but apparently that wasn't ok? Can you at least try to fix this then?

###Proposed Method 1 - Outside-In With Blocks
Jason Steffen's work is near perfect in optimizing the needed boarding time. However, if is tedious for the company, as they have to call every passenger separately. Therefore, I propose a combination of Steffen's work and the sectioned boarding:

*The passengers are divided in sections, and get a number according to their seating position (aisle: 3, middle: 2, window: 1).
*Those blocks are then called (similar to the currently used method) to get in line to board (e.g. F1, E1, ..., A1, F2, ..., A2, F3, ... A3).

This method keeps the organisation needed beforehand to a minimum, while still using the benefits of parallelized boarding. It is basically a combination of the rear-to-front, and the outside-in method.

####Implementation
As we have completely randomized passengers in the vector, we have to order them after their "column" first (window/aisle/middle), and then divide them into 3 groups according to their seating row.

####Performance (Needed Time)
<center>
 0 3 3 3 0 0 0 3 0 0 0 0 0 0 3 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 3 3 3 3 0 0 0 0 0 0
 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
 2 2 2 2 2 2 1 1 2 2 2 2 2 2 2 2 2 2 2 1 1 2 2 2 2 2 2 2 2 2 2 1 1 1 1 2 1 1 1 1
 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 3 0 0 0 0 0 0 0 0 0 0 0
</center>
The plane after the first 50 timesteps looks surprisingly similar to the Outside-In method. There aren't much more people sitting, but there are many people coming in in big blocks to sit down. It is especially remarkably that the window seats fill up extremely fast after some ramp-up time.

In the following graph, the relation between the number of groups the passengers are separated into, and the needed boarding time is shown for 3 different cases:
* The average (where every passenger carries 1 piece of luggage)
* The ideal case (nobody brings luggage into the plane)
* The worst case (everyone needs to store two pieces of luggage)
[[File:Boarding 1.PNG|500px|center|thumb|average boarding time from 100 simulations for 10 cases with 3 different parameters]]

First, we take a closer look into the average case:
The graph looks pretty much how we expect it to look: First of all, the boarding time decreases as the number of groups increases with a decreasing return to scale. This makes sense, as the carried luggage is still the limiting factor that stops people from getting to their place. This factor gets smaller when the number of groups increases because the whole process is more "ordered": less people are in their way.

At the worst case, we notice that the slope of the graph is steeper than in the average case. The reason for this is the bigger storing time for the luggage (two times as long).

In the ideal case, the slope of the graph is extremely small, as even big groups already only have small delays. When we get to 8 groups, we find the ideal time which is 241 timesteps. The reason that the time isn't the optimal one can be explained by a simple example, which can be extended to the whole plane:
* Assuming we have only two passengers  which are seated in the first two rows, we get the following patterns for the boarding process, depending on how they board the plane (only the first two rows are shown here):
**Case 1: Passenger 1 has the seat number 7, passenger 2 has the seat number 0
**Case 2: Passenger 1 has the seat number 0, passenger 2 has the seat number 7
<center>
 Case 1:         Case 2:
 00  00  00  |  00  00  00  00
 00  00  00  |  00  00  00  00
 00  00  00  |  00  00  00  00
 21  22  11  |  21  21  12  11
 00  00  00  |  00  00  00  00
 00  00  00  |  00  00  00  00
 00  00  33  |  00  30  30  33
</center>

In the first case, the two passengers need three timesteps to get to their respective places. In the second one, four are needed. This can be extended to the complete plane with 240 seats/passengers.

[[File:outside_in_grps.gif|500px|center|thumb|Basic outside in boarding method with 3*6 groups. (Depending on the browser, you have to click the thumbnail to see the animation)]]

####Customer Satisfaction

###Proposed Method 2
As mentioned above, the Steffen's method is theoretically beautiful, but practically unusable. The problem is that it is in the current boarding method very difficult to correlate the boarding order of passengers, and their seat. But what if, instead of assigning seats at the check-in, we decide to assign them at the boarding? We then have a perfect control over the correlation between these variables. In clear, as the passengers go through the gate to board the plane, we make a system that gives each one a them a ticket telling "your seat is ___", and give them the optimal seat given the present state of the boarding process. 

Although this might give us a better "boarding time", we have to keep in mind that the limiting factor with this method is probably (depending on the exact method) the time the passengers spend waiting at the gate for their tickets. Therefore the results we get from the simulation should be analyzed carefully.

#### Performance (Needed Time) 
It is obvious that we want to have a boarding time as low as possible, so that the time needed to assign the seats is the limiting factor of the achievable speed.

To get the boarding time as low as possible, we use the ideas of the already implemented methods, the main one being the parallelisation of the process (having as many people store their luggage at the same time). To achieve that goal, we just fill the plane up from the sides (as Jason does). For the individually travelling passengers, there are no problems with this approach. We might run into issues that we are not able to place groups of three together anymore, if too many people already boarded (and therefore no whole row is still empty). This could be solved by urging groups of more than two travelers to get into the line to board as soon as possible (or by modifying the process for individual passengers a tiny bit).

However, this leaves us with the problem that people (families, friends) want to sit together, and slow down the whole process by doing so. To determine the optimal way of placing groups, we try a few different approaches, and conduct further experiments on the best one.

* Placing the groups at the same place we would put individual passengers. This is obviously not ideal, but does the job for a first "benchmark"-experiment.
* By putting the groups more towards the rear of the plane, obstructing other passengers could be avoided as much as possible. (The boarding process in the front can continue like normal, while the two/three people in the back can store their luggage without obstructing anyone).
* If we forced the groups to board first, we could place them optimally. They would still disturb the ideal boarding process slightly, but less than they would if they boarded in the middle of the process.

The following code assigns the incoming passengers the seating place that corresponds to Steffen ideal boarding method (ignoring any grouping wishes):
<source lang="cpp">
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
</source>
The results are (not very surprisingly) similar (not exactly due to the randomness of the luggage-storing-time) to the ones of the ideal boarding method. No graph needed here.

#### Customer Satisfaction 

=Simulation=

###Objective
The primary objective of the simulation is to compare different boarding strategies, to have an idea of which strategy is optimal (qualitative), and by around how much it is better (quantitative). 

In order to have a reliable results, we need to be careful about defining properly the model of simulation. We obviously need to approximate a lot of things in order to simulate a boarding process, (for instance, choosing a discrete time), but if an extremely "non natural" hypothesis is made in the model of the simulation, there is a risk that the results will be not only quantitatively, but also qualitatively wrong. Indeed, a bad model could favor a particular strategy and could potentially make it perform better than another one, even if it wouldn't in reality.

After defining a proper simulation model, we will implement the different strategies defined above, and compare their performance. In particular, we will implement the currently used boarding strategies, more precisely the "Blocks" strategies (filling the plane from behind, or from the windows), and the "Random" strategy, and measure their performance in our simulation model to get a quantitative lower-bound of the performance our strategies should reach. We will also implement and measure the optimal strategy of Steffen, to get an idea of how far we are from the temporal optimality.

###Model
We are going to simulate the boarding process of the different methods on a few airplanes to see if the ideas work on all planes or only on specifically built ones. The simulation should output at least the needed time to board and some sort of measurement of the customer satisfaction.

####Different Airplane Layouts
It is important to see the performance of one boarding method on different airplanes (e.g. one vs. two doors, one vs. two aisles, different length). I suggest that we use [http://www.seatguru.com/ SeatGuru] to find the maps of two or three representative planes, and convert them into a readable format for our simulation.

Planes to be used (with links to the seat-map):
* boeing 777 ([http://www.seatguru.com/airlines/Emirates_Airlines/Emirates_Airlines_Boeing_777-300ER_3class_V2.php]): Two aisles, 1 door

####Passenger Generation
What characterizes a passenger (e.g. seating position in the airplane, does the person have luggage?, etc)
'''Objective''' of the simulation: time elapsed between start of boarding, and undocking/taxiing of the aircraft

In a first model, we can consider each passengers as individually independent agents, each carrying a luggage of around the same size that they will need to stow in the cabin above. The time necessary to store the luggage will be approximated by a Gaussian law for the moment. Later on, we will consider couples, families, etc... people who would like to queue and sit together.

A customer satisfaction could be defined as a function of the following features:
* Some customer might want to reserve a specific seat
* Some customers might want to sit on the window/aisle side
* Some customers wand to sit together with their family/partner

####Boarding

'''Components''' of the customer journey:
* Waiting time in gate until you're called
* Waiting time in line once you're called
* Scanning of boarding pass
* Walk between gate and aircraft-door
* Waiting for other people blocking you from reaching their seat
* Stowing luggage
* Have people get up so you can reach your seat
* Sitting down

We cannot do much about the things that happens before the actual boarding in the plane. In our current study, the objective is to find an optimal boarding strategy, i.e. a seat/boarding order correlation for the passengers that would optimize time and their satisfaction. Therefore, we will only consider the waiting time INSIDE the aircraft, which is the sum of the waiting of other passengers blocking you from reaching your seat, the stowing of the luggage and the waiting of people getting up so that you can reach your seat.

In a first model, we can define the boarding simulation model as follow:
* We only consider the evolution of passengers inside the aircraft, from the entrance till their respective seats.
* Each passenger occupies one space unit.
* A time unit corresponds to the time a passengers needs to make a step.

###Code
####Implementation
#####Assumptions
* A passenger takes one time unit to walk from one row to another.
* It is not possible to pass/overtake a passenger.
* It is not possible to pass a person which is storing their bags.
* It is not possible to pass a person which is currently waiting to sit down (if that person has to wait to be able to sit down).
* Storing luggage takes a random amount of time between 0 and 10 time units.
* Sitting down takes (and therefore blocking the aisle) 3 time units per blocking person (e.g. if you have a window seat, but the other two passengers are already sitting, you need 6 time units to sit down).

#####Passengers
The passengers currently have a few characteristics, which are all determined at their creation:
*the seat number (at first random)
*their current position
*the time they need to store their luggage
*their current status (sitting or not)

<source lang="cpp">
class passenger
{
public:
	passenger(){};
	passenger(bool _luggage, int _seat) : luggage(_luggage), seat(_seat), isSitting(false), currentPos(-1), sitTime(-1) { luggageTime = rand() / RAND_MAX * 10; };
 
	int getSitTime(){ return sitTime; };
	int getSeat(){ return seat; };
	int getPos(){ return currentPos; };
	bool getLuggage(){ return luggage; };
	bool getSeatingStatus(){ return isSitting; };
 
	void setSitTime(int s){ sitTime = s; };
	bool sitDown()
	{
		--sitTime;
		if (sitTime <= 0)
			return true;
 
		return false;
	}
	void changePos(int pos){ currentPos = pos; };
	void changeSeatingStatus(){ isSitting = true; };
	void assignSeat(int _seat){ seat = _seat; };
	bool storeLuggage()
	{
		--luggageTime;
		if (luggageTime <= 0)
			return true;
 
		return false;
	};
 
private:
	int currentPos;
	int seat;
 
	int sitTime;
	int luggageTime;
	bool luggage;
	bool isSitting;
};
</source>
The passengers are all generated in order, and are therefore still ordered in the beginning. Later on, we shuffle them, and rearrange them according to our boarding method.

<source lang="cpp">
std::vector<passenger> createPassengers(int n)
{
	std::vector<passenger> pass;
	int cnt = 0;
	for (int i = 0; i < n; ++i)
	{
		pass.push_back(passenger(true, i + cnt));
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
		int vrand = (float)rand() / RAND_MAX * (pass.size()-1);
		while (r[vrand] == -1)
		{
			vrand += 1;
			if (vrand == pass.size())
				vrand = 0;
		}
		r[vrand] = -1;
 
		pass_rand.push_back(pass[vrand]);
	}
 
	return pass_rand;
}
</source>
This method of mixing up does not give us a completely "random" distribution of the passengers, but the algorithm is guaranteed to finish in a more or less timely manner. It is random enough for us, as we mostly sort them later on anyways.

#####Boarding
For the boarding-process, the passengers are first put into their corresponding aisle (in the currently implemented plane, everyone goes to the one in the middle). After that, they begin to move forward, checking every row if they have to sit down here. If not, they just continue walking, if yes, they first store their luggage, and check if it's possible for them to sit down. In case they are the first passenger in their part of the row, they sit down, in the other case, they continue to block the aisle while waiting to sit down.

<source lang="cpp">
int board(std::vector<std::vector<int>> &plane, std::vector<passenger> &pass)
{
	int timer = 0;

	while (!checkSeating(pass))
	{
		for (int i = 0; i < pass.size(); ++i)
		{
			if (pass[i].getPos() == -1 && plane[0][3] == 1)
			{
				pass[i].changePos(3);
				plane[0][3] = 2;
			}

			if (!pass[i].getSeatingStatus() && pass[i].getPos() != -1)
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
					}
				}

				//Search aisle
				else
				{
					int row = pos / 7;
					if (row < plane.size())
					{
						if (plane[row+1][pos % 7] == 1)
						{
							plane[row][pos % 7] = 1;
							plane[row+1][pos % 7] = 2;

							pass[i].changePos((row + 1) * 7 + pos % 7);
						}
					}
				}
			}
		}

		timer += 1;
	}

	return timer;
}
</source>

#####Output
At the moment, the only output is the number of time-steps needed for the complete boarding (until everyone sits at the right place). However, it is possible to get different outputs like the number of people currently waiting, a "map" of the plane (where people are sitting, where people are standing).

'''Map:'''
Example of random boarding (after 50 timesteps):
<center>
 0 0 0 0 0 0 0 0 0 0 3 0 0 0 0 0 0 0 0 0 3 3 0 0 0 0 0 0 0 0 0 0 3 0 0 0 3 0 0 0
 3 3 0 0 0 0 0 0 0 0 3 0 0 3 0 3 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 3
 0 3 0 0 0 3 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
 1 2 2 2 2 2 2 2 2 2 2 2 2 2 2 1 2 1 2 1 1 1 2 2 2 2 2 1 1 1 1 1 1 1 1 2 1 1 2 1
 0 0 0 3 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 3 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
 0 0 0 0 0 3 0 0 0 3 0 0 0 0 0 0 0 3 0 3 0 0 0 0 0 0 0 3 0 0 0 0 0 0 0 0 0 0 0 0
 0 3 0 0 0 0 0 0 0 3 0 0 3 0 0 0 0 0 0 3 0 0 0 0 0 0 0 3 0 0 0 0 0 0 0 0 0 0 0 0
</center>
 0: Empty seat; 1: Empty aisle; 2: full aisle; 3: full seat.

#####ToDo
* Implementing the missing algorithms
*Analyzing (and adjusting) the timesteps for the different movements.
*Extending the code to different plane-layouts (should mostly work already).

=Visualization=
A visualization for the boarding process was needed to better analyze bottle necks in the boarding process, and analyze if the algorithm even does what it's supposed to.
The following program reads the output from a txt file that's produced from the cpp code found on the talk-page.

The visualization of the boarding process is color-coded after the following rules:
* black: Empty seats
* grey: Empty aisle
* red: passenger in the aisle
* orange: passenger waiting for people sitting in his way to get up
* yellow: sitting passengers

<source lang="cpp">
import gifAnimation.*;

GifMaker gifExport;
BufferedReader reader;
String line;

int s = 10;

void setup() {
    size(42 * (s+5), 9 * (s+5));
    frameRate(40);

    gifExport = new GifMaker(this, "inside_out_grps.gif");
    gifExport.setRepeat(0);             // make it an "endless" animation
    gifExport.setTransparent(255, 255, 255);    // black is transparent
    
    reader = createReader("test.txt");

}

void draw() {
  background(200);
  
  for(int j=0; j<40; ++j)
  {
    try {
      line = reader.readLine();
    } catch (IOException e) {
      e.printStackTrace();
      line = null;
    }
    if (line == null) {
      // Stop reading because of an error or file is empty
      noLoop(); 
      gifExport.finish(); 
    } else {
      
      String[] t = split(line, TAB);
      String[] pieces = split(t[1], ' ');
      
      for(int i=0; i<7; ++i)
      {
        int p = int(pieces[i]);
        
        switch(p){
          case 0:
            fill(0);
            break;
          case 1:
            fill(100);
            break;
          case 2:
            fill(#B40000);
            break;
          case 3:
            fill(#FAF200);
            break;
          case 4:
            fill(#FC7F00);
            break;
        }
        rect((j+1)*(s+5), (i+1)*(s+5), s, s);
      }
      //print('\n');
    }
  }

    gifExport.setDelay(1);
    gifExport.addFrame();
}

void mousePressed() {
    gifExport.finish();                 // write file
}

</source>

###Example
The following gif shows the boarding process without sorting the passengers beforehand.
[[File:rand_board.gif|500px|center|thumb|randomly boarding people - demonstration (Depending on the browser, you have to click the thumbnail to see the animation)]]

=Analysis=
{| class="wikitable sortable"
|-
 | Method (Name)
 |  colspan=2 | Needed Time ( 0 pieces of luggage)
 |  colspan=2 | Needed Time ( 1 pieces of luggage)
 |  colspan=2 | Needed Time ( 2 pieces of luggage)
 | colspan=2 |  Needed Time ( random pieces of luggage)
|-
! | -
! | Worst Case
! |  Best Case
! | Worst Case
! |  Best Case
! | Worst Case
! |  Best Case
! | Worst Case
! |  Best Case
|-
| Jason Steffen (1)
| ...
| ...
| ...
| ...
| ...
| ...
| ...
| ...
|-
| Jason Steffen (2): FFA
| ...
|-
| Random with assigned seats
| ...
|-
| Rear to front
| ...
|-
| Outside In
|...
|-
| Proposed (1): Outside in with groups
| ...
|-
| Proposed (2): Assignment at boarding
| ...
|}

Writing the questions towards the other contributors here so it gets seen: 
* How do we account for different parameters in the general analysis (worst/best case/ratio of the two)?
* Standard deviation needed for statistical purposes?
* Would that layout be decent?
* Are 100 simulations enough for a meaningful answer (bump it up to 1000 or even 10000)?
* Did anyone besides me run any simulations?

= Implementation =
This is where we'll be doing some actual product design. Once/if the simulations show indeed an improved performance, how will it be implemented? Possible examples: mockups, customer journey, software dummy (lay-out wise).

###Paper
####Existing Papers
* Analysis of airplane boarding via space-time geometry and random matrix theory<ref>http://arxiv.org/pdf/physics/0512020v1.pdf</ref>:<br/> Modeling of the boarding process in a strict mathematical way instead of a discrete event simulation (like we are doing it here).
* A study of the airline boarding problem<ref>http://www.sciencedirect.com/science/article/pii/S0969699708000513</ref>:<br/> A general discussion and summary of the different boarding processes in use. This paper looks at the financial side of the process, and analyzes the results of other studies. The authors come to the conclusion that switching to a different boarding style could save several millions per year, and additionally increase the customer satisfaction.
* Reducing passenger boarding time in airplanes: A simulation based approach<ref>http://www.sciencedirect.com/science/article/pii/S0377221701002946</ref>:<br/> Focused on short haul flights (and therefore small airplanes), this paper simulates a wide variety of boarding methods and analyzes them. Additionally, the issues faced at the implementation of the "individual call"-method are discussed: Comparisons to similar systems are made (delis).
* Experimental test of airplane boarding methods (Steffen)<ref>http://www.sciencedirect.com/science/article/pii/S0969699711000986</ref>:<br/> Steffen was able to conduct an experimental verification of his previous paper. He analyzed different boarding-methods in a mock fuselage with 72 passengers. The results strongly support Steffens proposed method, and show that block-boarding is one of the worst methods possible.
* A statistical mechanics model for free-for-all airplane passenger boarding (Steffen)<ref>http://scitation.aip.org/content/aapt/journal/ajp/76/12/10.1119/1.2982636</ref>:<br/> This paper discusses the free for all (FFA) boarding method using Boltzmann statistics. He then goes on to simulating his model with different parameters.
* Optimal boarding method for airline passengers<ref>http://ac.els-cdn.com/S0969699708000239/1-s2.0-S0969699708000239-main.pdf?_tid=e01e789a-feea-11e4-ba13-00000aacb35f&acdnat=1432124726_16f69e8a8022d51a82b900a3e7a4ff23</ref>:<br/> Using a Markov Chain Monte Carlo (MCMC) algorithm as a base to find the optimal boarding strategy, Steffen finds the optimal boarding strategy with an optimisazion-approach. He proceeds to compare his algorithm with other methods.

###Customer Journey 
Based on the choice of the above algorithm, not much would significantly change for the customers themselves (in a matter of airport experience). There are two possibilities for seat assignment.

#### Assignment at check-in 
At check-in would mean the smallest change for airlines at all. The system would only have to be slightly adjusted (the seat-assignment algorithm), and this would then directly be printed on the boarding pass. Downside: at the gate, the assistants will have to call the blocks in a structured way, which might mean a lot of microphone time and risks of misunderstanding.

#### Assignment at boarding
Seat assignment at the gate would solve the issues described in the check-in method: just before boarding the aircraft, the passenger is given a second boarding pass with his seat indicated. The machine would simply print the seat based on the current situation of the boarding process. 

But a problem arises: how should we take into account family or group seating? For this, we might need to implement a new strategy, with the following capacity: 
Every time a passengers comes at the gate, the algorithm asks if the passenger is alone or in a group, and in the latter case, asks how many people the group consists of. Then the algorithm will assign them the optimal seat, meaning a seat that would minimize the total boarding time, trying at the same time to let a group sit together. At any given time, the algorithm knows the seats already assigned, and the number of groups and their size, given at the moment of the check-in.
It has to follow Steffen's optimal method, but we have to make it flexible to allow the group-sitting.
//How will the gate-method take into account family/group seating?

###Real Life - Test

= References =
<references/>

= Declaration of Originality =
[[File:Vhartman_declaration-originality.pdf|200px]] 
[[File:Laurenth_boardingrev.pdf|Laurenth|200px]]
[[File:declaration_sonny_coaorg.pdf|200px]]


In order for the Meta project to work, please keep this at the bottom of the page.
[[MetaCategory:Research Project]]
[[Category:2015%20Entrepreneurial%20Risks]]
