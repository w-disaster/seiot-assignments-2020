package view;

import java.awt.Color;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.data.xy.XYDataset;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

public class GraphImpl implements Graph {

	private final JFrame frame;
	private final JPanel canvas;
	

	private final JLabel label;
	
	public GraphImpl(){

		this.label = new JLabel("hello");
		this.frame = new JFrame();
		this.canvas = new JPanel();
		
		this.canvas.add(label);
		this.frame.add(canvas);
        
		this.frame.pack();
		this.frame.setVisible(true);
	}
	

	@Override
	public void plot() {
		
	}

	@Override
	public void reset() {
		// TODO Auto-generated method stub

	}

}
