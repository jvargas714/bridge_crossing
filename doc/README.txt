Brief:
The program expects one argument and that is the path to the crossing configuration file. If the file cannot be found
or fails to be opened for processing the program will exit. A report will be generated of events described in config.

Dependencies:
LibYAML - A C library for parsing and emitting YAML dist 0.2.2
    - github: https://github.com/yaml/libyaml

Build Env:
    - cmake >= v3.10
    - autoconf
    - clang or gcc (tested with clang)
    - c++ standard 14

To Build:
chmod +x ./build.sh
$ ./build.sh

executable:
    bin/bridge_crossing

running application:
$ ./bin/bridge_crossing cfg/default_crossings.yaml

event configuration file:
    In the ./cfg directory there are configuration files that outline various combination of hikers and crossings
    The default cfg file outlined in the problem statement is cfg/default_crossings.yaml

    New cfg files can be created with varying amounts of hikers, consecutive crossings, can have consecutive
    hiker encounters etc etc.

Bridge Crossing Sim Configuration format:
    The format is pretty simple, the file is read in to the system from top to bottom. The outer most
    element is a sequence of events.
    Events:
        Two types of events can occur (encounter and a crossing).

        "encounter" event:
            is when hikers join the group and will be participating on the next bridge crossing. "hikers" is defined
            as a sequence of hikers who are defined by name and a speed attribute

        "crossing event":
            a crossing event is actually when the hiking team must cross the bridge. There is one relevant attribute
            length which defines the length of the bridge

    Custom configuration files can be created to further test the parser. Additional crossings, and hikers can be added
    to the team.

Program Description:
    From the program entry point (./src/main.cpp) the BridgeCrossingProcessor class takes in the fileName. To begin
    processing of this file simply call BridgeCrossingProcessor::processCrossings.

    File Processing Paradigm:
        - essentially the BridgeCrossingProcessor wraps the libyaml yaml parsing library. As we traverse through the
        configuration file different yaml events are encountered. Parser state is kept so that we know what data
        we are currently parsing. When hit an "encounter" event, we add new hikers to our team.
        When a "crossing" event is hit the current group of hikers is used to calculate the
        fastest time the hikers are able to get across the bridge (see assignment pdfs for criteria). The algorithm and
        report generation is implemented within the BridgeCrossingHandler.

        - The BridgeCrossingHandler crossing algorithm method and report generation methods have been made to be
        virtual. The BridgeCrossingHandler can be extended to implement different methods of report generation as well
        as experimenting with different crossing algorithm implementations.

    Bridge Crossing Algorithm Implementation:
        - We know from the description that the 2 people can cross the bridge at once, since it is dark they must take
        a flashlight with them. Because of this the flash light must returned to the beginning of the bridge inorder for
        the others to cross as well. Travel speed across the bridge is limited to the slowed of the two traversing the
        bridge.

        - a straight forward approach would be make the fastest person the designated torch holder. This person would
        be the one to return the torch as the cost associated with that return trip would be minimized.
        This strategy turns out not to be the most optimal we can observe this buy running through an example.

        we convert the speeds to time for ease of explanation (1 based indexing is used)
        t = [1, 2, 5, 10] << this list shows the time that would be needed to get across the bridge, by the approach
        outlined above t[0] will be the torchbearer.

        Going through the trips and time usage
        trip1: (1, 2) --> left side [5, 10] , right side [1, 2], timetaken: 2, total time: 2
        return trip (1) <-- left side [1, 5, 10], right side [2], timetaken: 1, total time: 3
        trip2: (1, 5) --> left side [10] , right side [1, 2, 5], timetaken: 5, total time: 8
        return trip (1) <-- left side [1, 10], right side [2], timetaken: 1, total time: 9
        trip3: (1, 10) --> left side [], right side [1, 2, 5, 10], timetaken: 10, total time: 19
        done

        looking at the time contributions of each person in Approach 1:
            t_total = 2 + 1 + 5 + 1 + 10 --> 2t[1] + t[2] + t[3] + t[4]

        Further looking at this solution we can see that there is a better solution. We see that the two slowest
        people both contributed to our total time effectively undermining the effort to minimize time usage. A better
        approach for this scenario is on trip2 instead of 1 escorting 5, we have the two slowests hikers go together.
        This way only one of them contribute to time usage. At this point we get a contribution of 10
        (since 5, and 10 went together).

        Going from trip2 it would look like this
        """" same trips as approach 1
        """"
        trip2: (5, 10) --> left side [10] , right side [1, 2, 5], timetaken: 10, total time: 13
        return trip (2) <-- left side [1, 10], right side [2], timetaken: 2, total time: 15
        trip3: (1, 2) --> left side [], right side [1, 2, 5, 10], timetaken: 2, total time: 17

        So after the two slowest make their way to the otherside of the bridge, we get hit with t[n] time contribution.
        The second fastest person t[2] (who must return the torch) comes back to escort t[1]. This approach resulted in
        a savings of 2 minutes.

        looking at the time contributions of each person in Approach 2:
        t1 + 2t2 + tN and 2t1 + tN−1 + tN
            t_total = 2 + 1 + 10 + 2 + 2 = 3t[2] + t[1] + t[4]

        Generalizing the above algorithm for n people, where n > 4
            Now the main differentiating factor at play here is how to handle to two slowest people. The two fastest will
            be utilized to return the torch. We end up having to work with optimizing (minimizing) time with 4 people
            at a time. This suggest a greedy like recursive solution where we only worry about the two slowest t[n] and
            t[n-1].

            For every recursive iteration we consider optimizing t[0], t[1], t[n], and t[n-1]. We take minimum time
            contribution from approach 1 and approach 2.

            Taking a look at the code snippet below we see the minimization of contribution, followed by a recursive
            call reducing the remaining people to be processed by 2
            The 2 approaches generalized for n hikers are shown below
            t_0 --> t1 + 2t2 + tN    --> case where second fastest comes back to escort fastest and slowest
            t_1 --> 2t1 + tN−1 + tN  --> case where fastest escorts persons n-1 and n-2

            from BridgeCrossingHandler.cpp :: _crossBridge()
            	// potential contribution
            	float t_0 = calcTime(hikers[0], crossingLength) +
            		2*calcTime(hikers[1], crossingLength) +
            		calcTime(hikers[n-1], crossingLength);

            	float t_1 = 2 * calcTime(hikers[0], crossingLength) +
            		calcTime(hikers[n-2], crossingLength) +
            		calcTime(hikers[n-1], crossingLength);

            	float timeTaken = std::min(t_0, t_1);  // take min contribution

            	// recursively calculate for remaining n-2 people
            	timeTaken += _crossBridge(hikers, crossingLength, offset+2);

            Note: at the top this recursive method base conditions are defined for n < 4 which are trivial when minimizing
            time contribution.

