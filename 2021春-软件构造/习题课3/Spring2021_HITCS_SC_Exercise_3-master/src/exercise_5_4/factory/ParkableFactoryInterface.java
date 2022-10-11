package exercise_5_4.factory;

import exercise_5_4.parkable.Parkable;

public interface ParkableFactoryInterface {
	Parkable create(String type, String plate, int width, String[] extraRegistrationInfo) throws Exception;

}
