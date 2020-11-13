package view;
/**
 * Shows the user the data from the experiment
 */
public interface Graph {

	/**
	 * Shows the graph on screen.
	 */
	void plot();
	
	/**
	 * Resets the canvas.
	 */
	void reset();
}
