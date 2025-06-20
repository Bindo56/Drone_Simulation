VR Drone Interaction System Documentation



https://github.com/user-attachments/assets/48b07152-8da0-4346-b14c-6703ef1f3922



Project Overview

This project is a VR-based drone interaction simulation built in Unreal Engine using C++. The core features include:
Flying Drone Pawn controllable in VR


+ 3D Interactive Objects (Actors) in the scene


+ UI Widget for displaying contextual information when interacting with objects


+ Highlight System for selected objects using an outline shader


+ Optimizations for performance, interaction, and visuals.

System Flow Summary

Player Movement:

+ Keyboard - WASD(Move Forward) , Right & Left Arrow (Turn) , E (Move Up Ascend) , Q(Move Down Decend) ,  Up & Down Arrow (Camera UP & Down) ,  F (Scan the Object) , Z (Close Scan Panel)

+  Controlled via VR controllers → input mapped to MoveForward, MoveRight, MoveUp, Turn, LookUpDown.

+  Player must be under 10 Unit to scan the Object 

+  Line Trace Start from Center of the Camera

⚙️ Code Modules Breakdown

1️⃣ FlyingDrone.cpp
Purpose:

 Defines the main drone pawn that the player controls. Supports movement, rotation, camera bobbing, object interaction, and UI display.

![image](https://github.com/user-attachments/assets/5ca1c2c6-51ca-459c-af94-d8a29e866e11)

Features:

+ Smooth acceleration and deceleration physics


+ Subtle floating/oscillation effect on the camera for realism


+ Forward raycast interaction system (line trace)


+ UI Widget instantiation on interaction


+ Material highlighting of objects on interaction


+ Built-in cleanup when closing widgets


Optimizations:

+ Normalizing input direction vectors to avoid performance spikes


+ Using interpolation (FMath::VInterpTo) for smooth motion and velocity decay


+ Drag damping to create smooth stopping without explicit logic


+ Optional CustomDepth use for highlighting (currently not active but available)



2️⃣ ActorCustomData (1 to 5)

Purpose:

 Custom 3D objects in the scene that the drone can interact with to get more information.

![image](https://github.com/user-attachments/assets/e066d67b-9ca1-4dc0-9450-0226e87663d9)


Common Features:

+ Customizable names (ElementName), material types, and inspection notes


+ Highlights via overlay material (OutlineShader) on interaction


+ Fully collision-enabled for ray tracing


+ Stencil buffer setup prepared for post-process outline (if needed)


Optimizations:

+ Uses constructor helpers to load assets once for efficiency


+ Sets collision channels only relevant for ray tracing to minimize unnecessary calculations




3️⃣ DroneInfoWidget

Purpose:

 Displays dynamic object information (Name, Material, Notes) in UI form during interactions.

Usage in Code:

+ Created dynamically with CreateWidget


+ Positioned relative to player view when activated


+ Cleanly destroyed when interaction ends or closed by the user


Optimizations Applied:

+ Widget only spawned on demand to prevent unnecessary memory use


+ IsInViewport() checked before removal to avoid errors or redundant operations



Object Detection:


 + Line Trace forward from the VR camera


+ On hit → ACustomActorDataBase::SetHighlight(true) applied


+ UI Widget spawned showing object details


Object Highlighting:

+ Material override with the OutlineShader material.


UI Interaction Cleanup:
 
+ CloseInfoWidget() → cleans up widget and removes highlight.



🖥️ Code Implementation Highlights

+ Reusable Actor Base Class → All actor variations inherit from one structure, supporting DRY principles.


+ Interactive Feedback → Visual + textual feedback for player clarity.


+ Clean Input Handling → Clear separation between input bindings and implementation functions.


+ Physics Realism → Damping forces, oscillations, and interpolation to create natural drone motion.



