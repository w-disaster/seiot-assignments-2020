package model;

/**
 * Class concern to a pair of two objects.
 * @author luca
 *
 * @param <X> the generic type
 * @param <Y> the generic type
 */
public class Pair<X, Y> {

    private X x;
    private Y y;

    public Pair(final X x, final Y y) {
        super();
        this.x = x;
        this.y = y;
    }

    public X getX() {
		return this.x;
	}

    public Y getY() {
		return this.y;
	}
	
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((x == null) ? 0 : x.hashCode());
		result = prime * result + ((y == null) ? 0 : y.hashCode());
		return result;
	}
	
	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		@SuppressWarnings("rawtypes")
		Pair other = (Pair) obj;
		if (x == null) {
			if (other.x != null)
				return false;
		} else if (!x.equals(other.x))
			return false;
		if (y == null) {
			if (other.y != null)
				return false;
		} else if (!y.equals(other.y))
			return false;
		return true;
	}

	@Override
	public String toString() {
		return "[" + x + ";" + y + "]";
	}
	
}
