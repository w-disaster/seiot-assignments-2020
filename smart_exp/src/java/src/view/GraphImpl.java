package view;

import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.LayoutManager;

import javax.swing.JFrame;
import javax.swing.JPanel;

import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.Plot;
import org.jfree.chart.plot.XYPlot;


public class GraphImpl implements Graph {

	private final JFrame frame;
	private final JPanel canvas;
	
	private final JFreeChart positionChart;
	private final JFreeChart speedChart;
	private final JFreeChart accelerationChart;
	
	private final Plot positionPlot;
	private final Plot speedPlot;
	private final Plot accelerationPlot;
	
	private final ChartPanel positionPanel;
	private final ChartPanel speedPanel;
	private final ChartPanel accelerationPanel;
	
	public GraphImpl(){

		this.frame = new JFrame();
		this.canvas = new JPanel();
		
		this.positionPlot = new XYPlot();
		this.positionChart = new JFreeChart(positionPlot);
		this.positionPanel = new ChartPanel(positionChart);
		
		this.speedPlot = new XYPlot();
		this.speedChart = new JFreeChart(speedPlot);
		this.speedPanel = new ChartPanel(speedChart);
		
		this.accelerationPlot = new XYPlot();
		this.accelerationChart = new JFreeChart(accelerationPlot);
		this.accelerationPanel = new ChartPanel(accelerationChart);
		
		
		this.canvas.add(positionPanel);
		this.canvas.add(speedPanel);
		this.canvas.add(accelerationPanel);
        
		
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
