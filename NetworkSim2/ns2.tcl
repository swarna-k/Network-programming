if { $argc != 2 } {
    puts "The Queue type and scenario number must be entered"
    puts "For example, DROPTAIL 2"
    puts "Please try again."
	exit 0
} else {
 set queue [lindex $argv 0]
 set scenario [lindex $argv 1]
}

if {$queue == "DROPTAIL"} {
	set queue "DropTail"
}

if {$queue == "RED"} {
	puts "RED selected"
Queue/RED set thresh_ 10
Queue/RED set maxthresh_ 15
Queue/RED set linterm_ 50
}

set filename "throughput_${queue}_${scenario}.txt"
puts "Opening $filename"
set fileId [open $filename w]
puts $fileId "Time\tThroughputH1\tThroughputH2\tThroughputH5"


#Create a simulator object
set ns [new Simulator]

#Open the nam trace file
set nf [open out.nam w]
$ns namtrace-all $nf

#Define a 'finish' procedure
proc finish {} {
        global ns nf fileId
        $ns flush-trace
	#Close the trace file
        close $nf
	#Close output file 
	close $fileId
	#Execute nam on the trace file
        exec nam out.nam &
        exit 0
}

proc record {} {
	global tcpsink0 tcpsink1 udpsink0 fileId 
	set ns [Simulator instance]
	set time .5
	set now [$ns now]
	set bw0 [$tcpsink0 set bytes_]
	set bw1 [$tcpsink1 set bytes_]
	set bw2 [$udpsink0 set bytes_]
	
	set tp0 [expr $bw0/$time*8/1000000]
	set tp1 [expr $bw1/$time*8/1000000]
	set tp2 [expr $bw2/$time*8/1000000]
	
	puts $fileId "$now\t[expr $tp0]\t[expr $tp1]\t[expr $tp2]"
	zerobytes
	$ns at [expr $now+$time] "record"
}

proc zerobytes {} {
	global tcpsink0 tcpsink1 udpsink0
	$tcpsink0 set bytes_ 0 
	$tcpsink1 set bytes_ 0
	$udpsink0 set bytes_ 0
}

proc scenario2 {} {
	puts "Selected Scenario 2"
	global ns R1 R2 H5 H6 queue udp0 udpsink0 cbr0
	set H5 [$ns node]
	set H6 [$ns node]
	
	$ns duplex-link $H5 $R1 10Mb 1ms $queue
	$ns duplex-link $R2 $H6 10Mb 1ms $queue
	
	
	$ns attach-agent $H5 $udp0
	$ns attach-agent $H6 $udpsink0
	
	$cbr0 attach-agent $udp0
	$cbr0 set packet_size_ 100
	$cbr0 set rate_ 1mb
	
	$ns connect $udp0 $udpsink0
	
	$ns at 0.0 "$cbr0 start"
}

#Create two nodes
set R1 [$ns node]
set R2 [$ns node]
set H1 [$ns node]
set H2 [$ns node]
set H3 [$ns node]
set H4 [$ns node]

$ns duplex-link $H1 $R1 10Mb 1ms $queue
$ns duplex-link $H2 $R1 10Mb 1ms $queue
$ns duplex-link $R1 $R2 1Mb 10ms $queue
$ns duplex-link $R2 $H3 10Mb 1ms $queue
$ns duplex-link $R2 $H4 10Mb 1ms $queue

set tcp0 [new Agent/TCP/Reno] 
set tcp1 [new Agent/TCP/Reno]
set tcpsink0 [new Agent/TCPSink]
set tcpsink1 [new Agent/TCPSink]

#only matters in case2
set udp0 [new Agent/UDP]
set udpsink0 [new Agent/Null]
set cbr0 [new Application/Traffic/CBR]

$ns attach-agent $H1 $tcp0
$ns attach-agent $H2 $tcp1
$ns attach-agent $H3 $tcpsink0
$ns attach-agent $H4 $tcpsink1

set ftp0 [new Application/FTP]
set ftp1 [new Application/FTP]

$ftp0 attach-agent $tcp0
$ftp1 attach-agent $tcp1

$ns connect $tcp0 $tcpsink0
$ns connect $tcp1 $tcpsink1

if {$scenario == 2} {
	scenario2
}


$ns at 29.5 "zerobytes"
$ns at 30.0 "record"
$ns at 0.0 "$ftp0 start"
$ns at 180.0 "$ftp0 stop"
$ns at 0.0 "$ftp1 start"
$ns at 180.0 "$ftp1 stop"
$ns at 180.0 "finish"

$ns run
