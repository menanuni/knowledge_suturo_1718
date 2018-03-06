#include <string>
#include <sstream>
#include <locale>
#include <ros/ros.h>
#include <knowledge_msgs/GraspIndividual.h>
#include <knowledge_common/prolog_util.h>
#include <json_prolog/prolog.h>
  
using namespace json_prolog;

std::string createQuery(std::string object_label)
{
  std::stringstream ss;
    ss << "find_grasp_pose(suturo_object:\'" 
       << object_label << "\',"
       << "Position, Quaternion)";
    
  return ss.str();
}

std::string translateToUnderscore(std::string label){

  std::stringstream rs;

  rs << std::tolower(label.at(0), std::locale());

  for(unsigned i=1; i<label.length(); ++i){
    if(std::isupper(label.at(i))){
      rs << "_" << std::tolower(label.at(i), std::locale());
    } else {
      rs << label.at(i);
    }
  }
  return rs.str();

}

bool find_grasp_pose(knowledge_msgs::GraspIndividual::Request &req, knowledge_msgs::GraspIndividual::Response &res)
{
    Prolog pl;
    PrologQueryProxy bdgs = pl.query(createQuery(req.object_label));
 
  for(PrologQueryProxy::iterator it=bdgs.begin();
      it != bdgs.end(); it++)
    {
      PrologBindings bdg = *it;
      res.grasp_pose.header.frame_id = "/" + translateToUnderscore(req.object_label);
	    res.grasp_pose.pose =  PrologUtil::prologBindingToPose(bdg, "Position", "Quaternion");
      return true;
	  }
    ROS_ERROR_STREAM("LABEL \"" << req.object_label << "\"NICHT ERKANNT! BITTE EINGABEN PRUEFEN!");
    return false; 
}
   
int main(int argc, char **argv)
{
  ros::init(argc, argv, "knowledge_grasp");
  ros::NodeHandle nh("~");
   
  ros::ServiceServer knowledge_grasp_service = nh.advertiseService("knowledge_grasp", find_grasp_pose);

  ros::spin();
   
  return 0;
}