class ExampleCh41PC extends SimplePC;


var float    PickDistance;


function ApplyForceRigidBody(Actor SelectedActor, Vector ImpulseDir,float ImpulseMag, Vector HitLocation)
{
    if (SelectedActor.IsA('KActor'))
    {
        WorldInfo.Game.Broadcast(self,"*** Thrown object " @ SelectedActor @ 
                                 ", ImpulseDir = " @ ImpulseDir @
                                 ", ImpulseMag = " @ ImpulseMag @
                                 ", HitLocation = " @ HitLocation);
        KActor(SelectedActor).ApplyImpulse(ImpulseDir,ImpulseMag, HitLocation);
    }
    else
    {
        WorldInfo.Game.Broadcast(self,"!!!ERROR Selected Actor " @ SelectedActor @ 
                                 " is not a KActor, you can not apply an impulse to this object!!!");
    }
}

function Actor PickActor(Vector2D PickLocation, out Vector HitLocation, out TraceHitInfo HitInfo)
{
    local Vector TouchOrigin, TouchDir;
    local Vector HitNormal;
    local Actor  PickedActor;
    local vector Extent;


    //Transform absolute screen coordinates to relative coordinates
    PickLocation.X = PickLocation.X / ViewportSize.X;
    PickLocation.Y = PickLocation.Y / ViewportSize.Y;
   
    //Transform to world coordinates to get pick ray
    LocalPlayer(Player).Deproject(PickLocation, TouchOrigin, TouchDir);
   
    //Perform trace to find touched actor
    Extent = vect(0,0,0);
    PickedActor = Trace(HitLocation, 
                        HitNormal,
                        TouchOrigin + (TouchDir * PickDistance), 
                        TouchOrigin, 
                        True, 
                        Extent, 
                        HitInfo);

   
    //Return the touched actor for good measure
    return PickedActor;
}

function bool SwipeZoneCallback(MobileInputZone Zone, 
                                float DeltaTime, 
                                int Handle,
                                ETouchType EventType, 
                                Vector2D TouchLocation)
{	
    local bool retval;
	
    local Actor PickedActor;
    local Vector HitLocation;
    local TraceHitInfo HitInfo;


    // Variables for physics
    local Vector ImpulseDir; 
    local float ImpulseMag; 


    retval = true;


    if (EventType == Touch_Began)
    {
        // If screen touched then pick actor
        PickedActor = PickActor(TouchLocation,HitLocation,HitInfo);
		
        WorldInfo.Game.Broadcast(self,"PICKED ACTOR = " @ 
                                 PickedActor @ 
                                 ", HitLocation = " @ HitLocation @
                                 ", Zone Touched = " @ Zone);

        // Set to roughly 45 degree angle
        ImpulseDir = Normal(Vector(Pawn.Rotation)) + vect(0,0,1); 
        ImpulseMag = 100;

        ApplyForceRigidBody(PickedActor,ImpulseDir,ImpulseMag,HitLocation);
		
    }
    else
    if(EventType == Touch_Moved)
    {	

    }		
    else
    if (EventType == Touch_Ended)
    {	
	
    }

    return retval;
}

function SetupZones()
{
    Super.SetupZones();

    // If we have a game class, configure the zones
    if (MPI != None && WorldInfo.GRI.GameClass != none) 
    {
        LocalPlayer(Player).ViewportClient.GetViewportSize(ViewportSize);

        if (FreeLookZone != none)
        {
            FreeLookZone.OnProcessInputDelegate = SwipeZoneCallback;
        }	
    }
}

defaultproperties
{
    PickDistance = 10000;
}

