#!/usr/bin/env python
import argparse
import rospy
import baxter_interface
from baxter_interface import CHECK_VERSION
class JointRecorder(object):
    def __init__(self, filename, rate):
        """
        Records joint data to a file at a specified rate.
        """
        self._filename = filename
        self._raw_rate = rate
        self._rate = rospy.Rate(rate)
        self._start_time = rospy.get_time()
        self._done = False        
        self._limb_right = baxter_interface.Limb("right")

    def _time_stamp(self):
        return rospy.get_time() - self._start_time
    def stop(self):
        """
        Stop recording.
        """
        self._done = True

    def done(self):
        """
        Return whether or not recording is done.
        """
        if rospy.is_shutdown():
            self.stop()
        return self._done

    def record(self):
        if self._filename:            
            joints_right = self._limb_right.joint_names()
            with open(self._filename, 'w') as f:
                f.write('time,')
                f.write(','.join([j for j in joints_right]) + '\n')
                while not self.done():                    
                    current_pose = self._limb_right.endpoint_pose()                      
                    endpoint_pose_position_right = [current_pose['position'].x,
                                           current_pose['position'].y,
                                           current_pose['position'].z,]
                    endpoint_pose_orientation_right = [current_pose['orientation'].x,
                                           current_pose['orientation'].y,
                                           current_pose['orientation'].z,
                                           current_pose['orientation'].w,]                     

                    f.write("%f," % (self._time_stamp(),))                    
                    f.write(','.join([str(x) for x in endpoint_pose_position_right]) + ',')
                    f.write(','.join([str(x) for x in endpoint_pose_orientation_right]) + '\n')
                    self._rate.sleep()
def main():
    epilog = """
Related examples:
  joint_position_file_playback.py; joint_trajectory_file_playback.py.
    """
    arg_fmt = argparse.RawDescriptionHelpFormatter
    parser = argparse.ArgumentParser(formatter_class=arg_fmt,
                                     description=main.__doc__,
                                     epilog=epilog)
    required = parser.add_argument_group('required arguments')
    required.add_argument(
        '-f', '--file', dest='filename', required=True,
        help='the file name to record to'
    )
    parser.add_argument(
        '-r', '--record-rate', type=int, default=20, metavar='RECORDRATE',
        help='rate at which to record (default: 100)'
    )
    args = parser.parse_args(rospy.myargv()[1:])
    print("Initializing node... ")
    rospy.init_node("rsdk_joint_recorder")
    print("Getting robot state... ")
    rs = baxter_interface.RobotEnable(CHECK_VERSION)
    print("Enabling robot... ")
    rs.enable()
    Recorder = JointRecorder(args.filename, args.record_rate)
    rospy.on_shutdown(Recorder.stop)
    print("Recording. Press Ctrl-C to stop.")
    Recorder.record()
    print("\nDone.")
if __name__ == '__main__':
    main()