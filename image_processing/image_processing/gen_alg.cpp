#include <iostream>

//  code
unsigned int grayencode(unsigned int g)
{
	return g ^ (g >> 1);
}

unsigned int graydecode(unsigned int gray)
{
	unsigned int bin;
	for (bin = 0; gray; gray >>= 1) {
		bin ^= gray;
	}
	return bin;
}



template <typename Image_type>
struct image_bread
{
	inline static Image_type image = Image_type();
	static void set_image(const Image_type& image) { image_bread<Image_type>::image = image;  prepare(image); }

	union
	{
		uint32_t whole[2];
		uint8_t chromo[2][4];
	};

	image_bread(uint32_t f, uint32_t s)
	{
		whole[0] = f;
		whole[1] = s;
	}

	bool operator !=(image_bread other)
	{
		return (whole[0] != other.whole[0] && whole[1] != other.whole[1]);
	}


	static image_bread crossover(std::pair<image_bread, image_bread> pair)
	{
		std::uniform_int_distribution<> dis(0, sizeof(char) * 3 * 8);
		int breakpoint = dis(gen) + 8;
		int right = (sizeof(char) * 4 * 8) - breakpoint;

		image_bread child = { (pair.second.whole[0] << breakpoint >> breakpoint) | (pair.second.whole[0] >> right << right),
							  (pair.second.whole[1] << breakpoint >> breakpoint) | (pair.second.whole[1] >> right << right) };

		// mutaion
		for (int chrom = 0; chrom < 2; chrom++)
			for (int i = 0; i < 32; i++)
				if (rand() % 100 < 20)
					child.whole[chrom] = child.whole[chrom] ^ 1 << i;


		for (int i = 0; i < 4; i++)
			child.chromo[0][i] %= 3;

		for (int i = 0; i < 4; i++)
			child.chromo[1][i] %= 16;

		return child;
	}


	static float fitnes(image_bread unit)
	{
		return image_rate(unit.apply());
	}

	Image_type apply()
	{
		// apply all operators with args to original image
		Image_type temp = image;


		// first transformation is hist aligment
		if (chromo[0][0] == 1)
			temp = auto_contrast(temp, chromo[1][0] * 0.01f);
		else if (chromo[0][0] == 2)
			temp = hist_alignment(temp);

		// apply convolution kernels
		for (int i = 1; i < 3; i++)
		{
			if (chromo[0][i] == 1)
				temp = box_filter(temp);
			else if (chromo[0][i] == 2)
				temp = sharp_filter(temp, chromo[1][i]);
		}

		return temp;
	}

	static bool check_res(image_bread unit, float fitnes)
	{
		return fitnes >= 1.0f ? true : false;
	}
};

// print (image bread) generation 
template <typename bread, size_t size>
void image_bread_print(const small::array<bread, size>& generation, float* fitnes_value)
{
	for (int i = 0; i < size; i++)
	{
		for (int cr = 0; cr < 2; cr++)
		{
			for (int j = 0; j < 3; j++)
				output("%d  ", (int)generation[i].chromo[cr][j]);

			output("\n");
		}

		output("%.4f \n\n", fitnes_value[i]);
	}
}


// cycles wasted in random loop
int cycles = 0;

// create new generaion
template <typename bread, size_t size>
std::tuple<small::array<bread, size>, int> evolution(const small::array<bread, size>& priv_generation)
{
	// calculate fitnes
	float fitnes_value[size];
	float fitnes_total = 0;
	float fitnes_avg = 0;


	// calculate fitnes value

	// single thread
	//for (int i = 0; i < size; i++)
	//{
	//	fitnes_value[i] = bread::fitnes(priv_generation[i]);
	//	fitnes_total += fitnes_value[i];
	//}

	// multi thread
	std::future<void> threads[size];
	for (int i = 0; i < size; i++)
	{
		threads[i] = workers.add_task([&, i]()
			{
				fitnes_value[i] = bread::fitnes(priv_generation[i]);
				fitnes_total += fitnes_value[i];
			});
	}
	
	for (int i = 0; i < size; i++)
		threads[i].get();


	// check was result reached or not
	for (int i = 0; i < size; i++)
	{
		if (bread::check_res(priv_generation[i], fitnes_value[i]))
		{
			image_bread_print(priv_generation, fitnes_value);
			return { priv_generation, i };
		}
	}
		

	fitnes_avg = fitnes_total / size;
	float fitnes_coef[size];
	// coef of parent validity
	for (int i = 0; i < size; i++)
		fitnes_coef[i] = fitnes_value[i] / fitnes_avg;


	// choose parents for new generation
	small::array<bread, size> parents;
	for (int i = 0; i < size; i++)
	{
		while (fitnes_coef[i] > 1.0f && parents.size < size)
		{
			fitnes_coef[i]--;
			parents.push_back(priv_generation[i]);
		}
	}

	// random loop (randomly take parents)
	while (parents.size < size)
	{
		std::uniform_real_distribution<> dis(0.0f, fitnes_total);
		for (int i = 0; i < size; i++)
		{
			cycles++;
			if (parents.size == size) break;
			if (fitnes_value[i] >= dis(gen))
				parents.push_back(priv_generation[i]);
		}
	}

	small::array<bread, size> children;

	// find closest(to result) idntity from privius generation
	int index = 0; float max = fitnes_value[0];
	for (int i = 1; i < size; i++)
		if (max < fitnes_value[i])
			max = fitnes_value[i], index = i;


	// best parent move to the next generation automaticly
	children.push_back(priv_generation[index]);


	// make pairs ramdomly to cross them and get children
	for (int i = 1; i < size; i++)
	{
		int first = rand() % size;
		int second = rand() % size;
		if (first == second) second = (second + 3) % size;
		children.push_back(bread::crossover(std::make_pair(parents[first], parents[second])));
	}

	
	image_bread_print(priv_generation, fitnes_value);
	return { children, -1 };
}



// core
template <typename bread, size_t size>
std::tuple<small::array<bread, size>, int> gen_alg(const small::array<bread, size>& start, int steps = 10, bool show_step = false)
{
	srand(time(0));
	int time = 0;
	small::array<bread, size> step = start;

	while (time < steps)
	{
		output("step %d ==============\n", time);

		// make new population
		auto [ret_step, finish] = evolution(step);
		step = ret_step;
		if (finish >= 0) return { step, finish };
		
		if (show_step)
			image_window(step[0].apply());

		time++;
	}

	return { step, 0 };
}