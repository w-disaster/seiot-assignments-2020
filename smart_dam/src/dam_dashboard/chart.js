const chartColor = '0, 149, 255';

var ctx = document.getElementById('chart').getContext('2d');

var data = {
    labels:[1,2,3,4],
    datasets: [{
        label: 'Level of Water',
        lineTension: 0,
        data: [10,100,50,70],
        backgroundColor: 'rgba('+chartColor+', 0.2)',
        borderColor: 'rgba('+chartColor+', 1)',
        borderWidth: 1,
        pointHoverRadius: 10,xAxesID: 'Time'
    }],
};

var options = {
    scales: {
        yAxes: [{
            ticks: {
                max:100,
                min:0,
                beginAtZero: true
            },
            scaleLabel: {
                display: true,
                labelString: 'Water Level Percentage'
              }
        }],
        xAxes: [{
            ticks: {
                beginAtZero: false
            },
            scaleLabel: {
                display: true,
                labelString: 'Time of Measurement'
              }
        }]
    }
};

var myChart = new Chart(ctx, {
    type: 'line',
    data: data,
    options: options
});