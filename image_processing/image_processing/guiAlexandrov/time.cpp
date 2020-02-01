

high_resolution_clock::time_point init_time;

float get_time()
{
	high_resolution_clock::time_point now = high_resolution_clock::now();
	duration<float>	dur = duration_cast<duration<float>>(now - init_time);
	return dur.count();
}


struct Time
{
	float init_time;

	Time()
	{
		init_time = get_time();
	}

	~Time()
	{
		float elapsed = init_time - get_time();
		doutput("%f\n", elapsed);
	}
};