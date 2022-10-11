package exercise_5_4.parkable;

import exercise_5_4.state.OnroadState;
import exercise_5_4.state.State;

public abstract class ConcreteParkable implements Parkable {
	private String plate;
	private int width;
	protected double pricingUnit;
	protected double price;

	private State state = OnroadState.getInstance();

	@Override
	public double getPricingUnit() {
		return pricingUnit;
	}

	@Override
	public double getPrice() {
		return price;
	}

	public ConcreteParkable(String plate, int width) {
		this.plate = plate;
		this.width = width;
	}

	@Override
	public int getWidth() {
		return width;
	}

	@Override
	public String getPlate() {
		return plate;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		ConcreteParkable other = (ConcreteParkable) obj;
		if (plate == null) {
			if (other.plate != null)
				return false;
		} else if (!plate.equals(other.plate))
			return false;
		if (width != other.width)
			return false;
		return true;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((plate == null) ? 0 : plate.hashCode());
		result = prime * result + width;
		return result;
	}

	@Override
	public State getState() {
		return this.state;
	}

	@Override
	public void setState(State state) {
		this.state = state;
	}

	@Override
	public String toString() {
		return plate + " (" + width + ", " + state + ")";
	}
}
