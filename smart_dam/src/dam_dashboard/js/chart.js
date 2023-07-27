const NRIL = 20;
const MIN_WL = 4;
const MAX_WL = 5;
const T = 1000;
let date;

/* Chart config */
const labels = [];

const data = {
    labels: labels,
    datasets: [{
        label: 'Water level',
        backgroundColor: 'rgb(65,105,225)',
        borderColor: 'rgb(65,105,225)',
        data: [],
    }]
};

const config = {
    type: 'line',
    data,
    options: {
        scales: {
            y: {
                min: MIN_WL,
                max: MAX_WL
            }
        }
    }
};

/* Chart object */
const riverChart = new Chart(document.getElementById('riverChart'), config);

/* Function to add data to the tail */
function addData(chart, label, data) {
    console.log(typeof label);
    if(typeof label === "string"){
        chart.data.labels.push(label);
        chart.data.datasets.forEach((dataset) => {
            dataset.data.push(data);
        });
        chart.update();
    }
}

/* Function to remove data from the head */
function removeData(chart) {
    chart.data.labels.shift();
    chart.data.datasets.forEach((dataset) => {
        dataset.data.shift();
    });
    chart.update();
}

/* Function to return a Date object from the
    received timestamp string */
function formatData(timestamp){
    if (typeof timestamp === "string"){
        const dateTimeParts = timestamp.split(' '),
        timeParts = dateTimeParts[1].split(':'),
        dateParts = dateTimeParts[0].split('-');
    
    return new Date(dateParts[0], parseInt(dateParts[1], 10) - 1, 
            dateParts[2], timeParts[0], timeParts[1], timeParts[2]);
    }
    return new Date();
}

/* This function is called every T ms and fetchs the data from Dam Service using AJAX.
    Note that T is indipendent from the sampling rate of the Remote Hydrometer, it can
    be set smaller or higher and the data is always received.
    This is done by locally setting the timestamp of the last received sample and subsequently
    request for data onwards */
function getData(){
    $.ajax({
        dataType: "json",
        /* Retrieves data by asking for those after the last fetched date */
        url: '/api/data/t=' + date.getTime(),
        type: 'GET',
        success: function(data){
            console.log(data);
            if(data.length > 0){
                $.each(data, function(i){                    
                    console.log(data[i]);
                    const dataDate = formatData(data[i]["Timestamp"]);
                    /* Print data to the chart if State != NORMAL and remove one
                        point from the head if the total number exceeds NRIL */
                    if(data[i]["State"] != "NORMAL"){
                        if(labels.length == NRIL){
                            removeData(riverChart);
                        }
                        addData(riverChart, dataDate.toUTCString(), data[i]["WaterLevel"]);
                    }
                });
                /* Update the date */
                date = formatData(data[data.length - 1]["Timestamp"]);
                /* Set the other values on the DOM*/
                $("#state").html(data[data.length - 1]["State"]);
                $("#opening").html(data[data.length - 1]["DamOpening"]);
                $("#mode").html(data[data.length - 1]["DamMode"]);
            }
        },
        complete: function(data){
            setTimeout(getData, T);
        }
    });
}

$(document).ready(function(){
    /* Get the last data sampled */
    $.ajax({
        dataType: "json",
        url: '/api/data/t=0',
        type: 'GET',
        success: function(data){        
            /* We set the variable date in order to
            request data related to subsequent periods */
            date = formatData(data["Timestamp"]);
            if(data["State"] != "NORMAL"){
                /* Print data to the chart */
                addData(riverChart, date.toUTCString(), data["WaterLevel"]);
            }
            /* Set the other values on the DOM */
            $("#state").html(data["State"]);
            $("#opening").html(data["DamOpening"]);
            $("#mode").html(data["DamMode"]);
        },
        complete: function(data){
            setTimeout(getData, T);
        }
    });
});