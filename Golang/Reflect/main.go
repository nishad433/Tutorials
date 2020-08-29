package main

import (
	"fmt"
	"reflect"
        "time"
)

func produce(ch chan<- string, i int) {
	time.Sleep(time.Duration(i) * time.Second)
	for j := 0; j < 5; j++ {
		ch <- fmt.Sprint(i*10 + j)
	}
	close(ch)
}

func main() {
	numChans := 4

	//I keep the channels in this slice, and want to "loop" over them in the select statemnt
	var chans = []chan string{}

	for i := 0; i < numChans; i++ {
		ch := make(chan string)
		chans = append(chans, ch)
		go produce(ch, i+1)
	}

	cases := make([]reflect.SelectCase, len(chans))
	for i, ch := range chans {
		cases[i] = reflect.SelectCase{Dir: reflect.SelectRecv, Chan: reflect.ValueOf(ch)}
	}

	remaining := len(cases)
	for remaining > 0 {
		chosen, value, ok := reflect.Select(cases)
                fmt.Printf("choosen=%v value=%v ok=%v\n\n",chosen, value, ok)
		if !ok {
			// The chosen channel has been closed, so zero out the channel to disable the case
			cases[chosen].Chan = reflect.ValueOf(nil)
			remaining -= 1
			continue
		}

		fmt.Printf("Read from channel %#v and received %s\n", chans[chosen], value.String())
	}
}

