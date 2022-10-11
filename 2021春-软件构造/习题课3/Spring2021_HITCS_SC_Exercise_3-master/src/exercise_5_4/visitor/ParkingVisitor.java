package exercise_5_4.visitor;

import exercise_5_4.field.ParkingField;

public interface ParkingVisitor {

	public double visit(ParkingField pf);

}
