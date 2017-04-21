#include "main.h"

int main(int argc, char ** argv)
{


	// set gpu_hog
	Args args;
	args.hit_threshold = 0.9;
	args.hit_threshold_auto = false;
	args.gr_threshold = 6;
	args.scale = 1.05;
	
	// args.src_is_video = true;
	// args.src = VIDEOFILE;


	App app(args);

	vector<vector<int > > rows;
	char query[] = "SELECT * FROM tracking;";
	db.selectTracking(query, rows);

	for (int i = 0; i < rows.size(); i++)
	{
		for (int j = 0; j < rows[i].size(); j++)
		{
			printf("%d ", rows[i][j]);
		}
		printf("\n");
	}
	//compareSimilarity(app);
	//buildTrajectory(app);
	//showTracklet(app);
	//detectionBasedTracking(app);
	//detectionAndTracking(app);

	// App app(args);
	// app.run();
	
	return 0;
}