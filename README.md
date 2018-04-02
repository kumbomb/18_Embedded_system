# 18_Embedded_system

Organizing content about Embedded_system class on CBNU

# Mini_Project

I made a kind of security system.

If it is necessary to limit access to other people, such as in a restricted area, then it is necessary to monitor the access of people. 

For the overall system, it is a structure in which, with security enabled, other people approach it and, over time, sound the alarm phase at its last level. And an alarm is sent to the manager to alert the outsider to approach. 

So I created a mini-project using ultrasonic sensor, leds, servo motor, buttons and soundboard used in the exercise to implement these systems.

The overall system operation process is as follows.

At first, the security is activated by pressing the Set Up button.

Secondly, the ultrasonic sensors detect the approach. Measure the time if someone approach it within 30 cm. Security level is green from 0 to 10. Security level is yellow from 11 to 20. Security level is red from 20 to 30. Since then, a waring tone has been emitted. and door is locked.

Third, if you touch free button, then alarm is lifted and door is unlock and you will revoke security.

demo video link is https://youtu.be/RajMIe1fL68
