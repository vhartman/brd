#ifndef PASSENGER_H
#define PASSENGER_H

class passenger
{
public:
	passenger(){};
	passenger(int _luggage, int _seat) : luggage(_luggage), seat(_seat), isSitting(false), currentPos(-1), sitTime(-1) { luggageTime = (float)rand() / RAND_MAX * 10 * _luggage; };

	int getSitTime(){ return sitTime; };
	int getSeat(){ return seat; };
	int getPos(){ return currentPos; };
	bool getLuggage(){ return luggage; };
	bool getSeatingStatus(){ return isSitting; };

	void setSitTime(int s){ sitTime = s; };
	bool sitDown()
	{
		if (sitTime <= 0)
			return true;

		--sitTime;

		return false;
	}
	void changePos(int pos){ currentPos = pos; };
	void changeSeatingStatus(){ isSitting = true; };
	void assignSeat(int _seat){ seat = _seat; };
	bool storeLuggage()
	{
		--luggageTime;
		if (luggageTime <= 0 || luggage == 0)
			return true;

		return false;
	};

private:
	int currentPos;
	int seat;

	int sitTime;
	int luggageTime;
	int luggage;
	bool isSitting;
};

#endif