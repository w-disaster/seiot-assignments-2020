const chartColor = '0, 149, 255';
const nMeasurements = 20;

//data sets ( labels -> x | data -> y )
var data = {
    labels:[],
    datasets: [{
        label: 'Level of Water',
        lineTension: 0,
        data: [],
        backgroundColor: 'rgba('+chartColor+', 0.2)',
        borderColor: 'rgba('+chartColor+', 1)',
        borderWidth: 1,
        pointHoverRadius: 10,xAxesID: 'Time'
    }],
};

//options mostly style
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

Number.prototype.allertToStatus = function(){
    status = "";
    color = "";

    switch (this.valueOf()) {
        case 0:
            status = "NORMALE";
            color = "primary";
            break;
        case 1:
            status = "PRE-ALLARME";
            color = "warning";
            break;
        case 2:
            status = "ALLARME";
            color = "danger";
            break;
        default:
            status = "Errore";
            color = "danger";
            break;
    }
    
    return {
        status:status,
        color:color
    };
}

function findLvl(className) {
    return className.includes("lvl-");
}


$(function(){
    //! FOR TESTING
        const allertLevel = 0
    //!------------

    const status = allertLevel.allertToStatus().status;
    const color = allertLevel.allertToStatus().color;

    $("div#state > p.value").html('<span class="text-'+color+'">' + status+'</span>');

    $("div.data").each(function() {
       showLvl = $(this).attr("class").split(/\s+/).find(findLvl).split("-")[1];
       if(showLvl > allertLevel){
           $(this).hide();
       }else{
           $(this).show();
       }
    });
    //! FOR TESTING

    for (let i = 0; i < 20; i++) {
        data.labels.push("2020-"); // adds X    
    }
    
    data.datasets[0].data.push(20); // adds Y
    //!------------

    //canvas element
    var ctx = document.getElementById('chart').getContext('2d');

    // chart creation
    var myChart = new Chart(ctx, {
        type: 'line',
        data: data,
        options: options
    });
});

