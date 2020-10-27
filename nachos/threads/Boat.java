package nachos.threads;

import nachos.ag.BoatGrader;

public class Boat {
	static BoatGrader bg;
	static int numAdultsinOahu;
	static int numChildreninOahu;
	static int numAdultsinMolokai;
	static int numChildreninMolokai;
	static Lock finishLock = new Lock();
	static Condition finishWork = new Condition(finishLock);
	static Lock waitLock = new Lock();
	static Condition waitPeople = new Condition(waitLock);
	static boolean finished = false;
	static int boatPosition = 0;
	static int childOnBoat = 0;

	public static void selfTest() {
		BoatGrader b = new BoatGrader();

		System.out.println("\n ***Testing Boats with only 2 children***");
		begin(3, 3, b);

		// System.out.println("\n ***Testing Boats with 2 children, 1 adult***");
		// begin(1, 2, b);

		// System.out.println("\n ***Testing Boats with 3 children, 3 adults***");
		// begin(3, 3, b);
	}

	public static void begin(int adults, int children, BoatGrader b) {
		// Store the externally generated autograder in a class
		// variable to be accessible by children.
		bg = b;

		// Instantiate global variables here

		// Create threads here. See section 3.4 of the Nachos for Java
		// Walkthrough linked from the projects page.

		finishLock.acquire();

		numAdultsinOahu = adults;
		numChildreninOahu = children;
		numAdultsinMolokai = 0;
		numChildreninMolokai = 0;
		finished = false;
		boatPosition = 0;
		childOnBoat = 0;

		Runnable r1 = new Runnable(){
			public void run(){
				AdultItinerary();
			}
		};
		for(int i = 0; i < adults; i++){
			KThread t = new KThread(r1);
			t.setName("Adult Thread" + i);
			t.fork();
		}
		
		Runnable r2 = new Runnable(){
			public void run(){
				ChildItinerary();
			}
		};
		for(int i = 0; i < children; i++){
			KThread t = new KThread(r2);
			t.setName("Child Thread" + i);
			t.fork();
		}

		while(numChildreninOahu + numAdultsinOahu > 0){
			finishWork.wake();
			finishWork.sleep();
		}
		finished = true;
		//finishWork.sleep();
		
	}

	static void AdultItinerary() {
		waitLock.acquire();

		bg.initializeAdult(); // Required for autograder interface. Must be the first thing called.
		// DO NOT PUT ANYTHING ABOVE THIS LINE.

		int position = new Integer(0);

		while(!finished){
			if(position == 0 && boatPosition == 0){
				if(numChildreninOahu == 1 && childOnBoat == 0){
					numAdultsinOahu -= 1;
					bg.AdultRowToMolokai();
					numAdultsinMolokai += 1;
					position = 1;
					boatPosition = 1;
				}
			}
			waitPeople.wake();
			waitPeople.sleep();
		}

		waitPeople.sleep();
	}

	static void ChildItinerary() {
		waitLock.acquire();

		bg.initializeChild(); // Required for autograder interface. Must be the first thing called.
		// DO NOT PUT ANYTHING ABOVE THIS LINE.

		int position = new Integer(0);

		while(!finished){
			if(position == 0){
				if(boatPosition == 0){
					if(numChildreninOahu >= 2 || numAdultsinOahu == 0 || childOnBoat == 1){
						numChildreninOahu -= 1;
						numChildreninMolokai += 1;
						position = 1;
						if(childOnBoat == 0){
							childOnBoat = 1;
							bg.ChildRowToMolokai();
						}
						else{
							childOnBoat = 0;
							bg.ChildRideToMolokai();
							boatPosition = 1;
						}
					}
					finishLock.acquire();
					finishWork.wake();
					finishLock.release();
				}
			}
			else{
				if(boatPosition == 1){
					numChildreninMolokai -= 1;
					bg.ChildRowToOahu();
					numChildreninOahu += 1;
					position = 0;
					boatPosition = 0;
				}
			}
			waitPeople.wake();
			waitPeople.sleep();
		}

		waitPeople.sleep();
	}

	static void SampleItinerary() {
		// Please note that this isn't a valid solution (you can't fit
		// all of them on the boat). Please also note that you may not
		// have a single thread calculate a solution and then just play
		// it back at the autograder -- you will be caught.
		System.out.println("\n ***Everyone piles on the boat and goes to Molokai***");
		bg.AdultRowToMolokai();
		bg.ChildRideToMolokai();
		bg.AdultRideToMolokai();
		bg.ChildRideToMolokai();
	}

}
