<?php
//TODO: prendere con ajax i dati ogni tot tempo
?>

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">

    <!-- Latest compiled and minified CSS -->
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" integrity="sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u" crossorigin="anonymous">

    <title>Dam Dashboard</title>
</head>

<body>
    <header>
        <h1>Dam Dashboard</h1>
    </header>

    <main>
        <!--//TODO: cercare di generare automaticamente -->
        <div id="state">
            <p>Dam Status:</p>
            <span></span>
        </div>

        <div id="w-lvl">
            <p>Water Level:</p>
            <span></span>
        </div>

        <div id="d-lvl">
            <p>Dam Level:</p>
            <span></span>
        </div>

        <div id="context">
            <p>Dam Control Mode:</p>
            <span></span>
        </div>

        <div id="chart">
            <p>Char of the last N measurments</p>
            <canvas></canvas>
        </div>
    </main>
</body>

</html>