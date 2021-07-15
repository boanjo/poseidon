# MySkyEye Web App
To give a good overview of the current status of my survillance system i have create a web application. At first i created a web app in Erlang / Nitrogen which is wonderful in the way you can manage the webserver with zero downtime. However i wanted to bundle this complete application using containers and then the overhead became a bit too big for my taste. So i decided to go for python and Flask which is nice and simple and enough for this kind of application. If anyone is interested in the Erlang solution give me a message and i'll upload it somewhere. 

![1](https://github.com/epkboan/epkboan.github.io/blob/master/myskyeye_dashboard.JPG?raw=true "Dashboard")

There is a highcharts graph showing all the objects that have been detected during motion (more than 5 frames during 10sec). Tensorflow often detects object that are not really there but i don't filter them out as it's fun to observe all the faulty predictions and try to figure out why there was firepost or a surfboard right outside my door :-). You can go back in time and show the motion per day and also select just persons or animals etc. Click on the items to replay the movie!

The next part is live mjpeg feeds from the cameras (One for the real time motion with annotations) and one with plain video. NOTE! these streams are using MJPG (To be able to view the page in any type of browser or device) and will quickly use more bandwidth if displayed continously.

The last part of the page is showing the last 16 (configure it to your needs) MUG shots ("Best" image with max persons in the picture). Click to replay the movie.

## Building
Edit the env.list (common for all the apps) and update any specific URL:s, ports etc in the docker.txt. Then do "source docker.txt" or run the commands manually

Navigate to the ipaddress and the port you choose to expose /dashboard. (in the docker.txt sample 8997) i.e. http://localhost:8997/dashboard
