package exercise_5_4.visitor;

import java.text.NumberFormat;

import exercise_5_4.field.ParkingField;

public class PercentageVisitor implements ParkingVisitor {

	@Override
	public double visit(ParkingField pf) {
		int numCars = 0;
		for (Integer i : pf.status().keySet()) {
			if (!pf.status().get(i).equals(""))
				numCars++;
		}
		int numLots = pf.getNumberOfLots();

		double percentage = (double) numCars * 1.0 / numLots;

		NumberFormat percentFormat = NumberFormat.getPercentInstance();
		percentFormat.setMaximumFractionDigits(2);
		percentFormat.setMinimumFractionDigits(2);

		System.out.println("Parking lot is " + percentFormat.format(percentage) + " full");
		return percentage;
	}

}
