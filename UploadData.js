// Upload datastream from serial port to plot.ly

var serialport = require('serialport'),
    plotly = require('plotly')('harriwijaya','yzh4xouy7k');
var token = ['bnfyqpllzu',
             'u35tgy6vdi',
			 'i4njtalvk8',
			 '8knco02e5r',
			 '8kmjg8ir44',
			 '2joqczwxsu'];
//

var portName = 'COM5';//'COM9'; //'/dev/tty.usbmodem1411';
var sp = new serialport.SerialPort(portName,{
    baudRate: 9600,
    dataBits: 8,
    parity: 'none',
    stopBits: 1,
    flowControl: false,
    parser: serialport.parsers.readline("\r\n")
});

// helper function to get a nicely formatted date string
function getDateString() {
    var time = new Date().getTime();
    // 32400000 is (GMT+9 Japan)
    // for your timezone just multiply +/-GMT by 36000000
    var datestr = new Date(time +7200000).toISOString().replace(/T/, ' ').replace(/Z/, '');
    return datestr;
}


var trace1 = {
	x:[],
	y:[],
	stream:{token:token[0], maxpoints: 500}
};
var trace2 =
{
	x:[],
	y:[],
	stream:{token:token[1], maxpoints: 500}
}
var trace3 =
{
	x:[],
	y:[],
	stream:{token:token[2], maxpoints: 500}
}
var trace4 =
{
	x:[],
	y:[],
	stream:{token:token[3], maxpoints: 500}
}
var trace5 =
{
	x:[],
	y:[],
	stream:{token:token[4], maxpoints: 500}
}


var initdata = [trace1, trace2, trace3, trace4, trace5];
var initlayout = {fileopt : "extend", filename : "sensor-multi-test"};

var data_sensor_last = [0,0,0,0,0];

plotly.plot(initdata, initlayout, function (err, msg) {
    if (err) return console.log(err)

    console.log(msg);
    var stream1 = plotly.stream(token[0], function (err, res) {
        console.log(err, res);
    });
	var stream2 = plotly.stream(token[1], function (err, res) {
        console.log(err, res);
    });
	var stream3 = plotly.stream(token[2], function (err, res) {
        console.log(err, res);
    });
	var stream4 = plotly.stream(token[3], function (err, res) {
        console.log(err, res);
    });
	var stream5 = plotly.stream(token[4], function (err, res) {
        console.log(err, res);
    });
	
	
	
    sp.on('data', function(input) {
        //if(isNaN(input) || input > 1023) return;
	
	var input_splitted = input.split(/[\s,]+/);
	console.log(input_splitted); //debug
	var data_sensor = [[0,0],[0,0],[0,0],[0,0],[0,0]]; // owh... javascript
	var i;

	for(i=0; i<5; i++)
	{
		data_sensor[i][0] = input_splitted[i+1].split(/(Y|N)/)[1]; // status
		data_sensor[i][1] = input_splitted[i+1].split(/(Y|N)/)[2]; // value
		
	}
	
	var data_plot = [0,0,0,0,0];
	
	for(i=0; i<5; i++)
	{
		if(data_sensor[i][0] == 'Y')
		{
			data_plot[i] = data_sensor[i][1];
			
			// SPECIAL TREATMENT:
			// for frequency, divide by 100000
			// for temperature, /10
			switch(i)
			{
				case 3:
					//data_plot[3] = data_plot[3] / 1000000;
					// take only last 2 digits (Decimal) ==> revise: 3rd and 2nd last digit
					var tempdata = 0;
					tempdata = data_plot[3]/1000;
					tempdata = Math.floor(tempdata); // remove dec points
					data_plot[3] = data_plot[3] - ((tempdata)*1000);
					data_plot[3] = data_plot[3] / 10; // 1 decimal point
					break;
				case 4:
					data_plot[4] = data_plot[4] / 10;
					break;
				default:
					break;
			}
			
			
			data_sensor_last[i] = data_plot[i];
		}
		else{
			data_plot[i] = data_sensor_last[i];
		}
	}
	
	
    var streamObject1 = JSON.stringify({ x : getDateString(), y : data_plot[0] });
    console.log(streamObject1);
    stream1.write(streamObject1+'\n');
	var streamObject2 = JSON.stringify({ x : getDateString(), y : data_plot[1] });
    console.log(streamObject2);
    stream2.write(streamObject2+'\n');
	var streamObject3 = JSON.stringify({ x : getDateString(), y : data_plot[2] });
    console.log(streamObject3);
    stream3.write(streamObject3+'\n');
	var streamObject4 = JSON.stringify({ x : getDateString(), y : data_plot[3] });
    console.log(streamObject4);
    stream4.write(streamObject4+'\n');
	var streamObject5 = JSON.stringify({ x : getDateString(), y : data_plot[4] });
    console.log(streamObject5);
    stream5.write(streamObject5+'\n');
    });
});