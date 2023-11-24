package main

import (
	"flag"
	"fmt"
	"net/http"
	"os"
	"strconv"
	"strings"
	"time"

	"github.com/mistsys/mist_go_utils/net/ethernet"
)

var http_port = flag.Int("http-port", 1234, "http api port number")

const SocketOpenRetryCnt = 5

func Dump_PcapSta(w http.ResponseWriter, mac ethernet.MAC) {
	fmt.Fprintf(w, "Mac %v\n", mac)
}

func DumpPcapSta(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "text/plain; charset=utf-8")
	macStr := r.URL.Query().Get("mac")
	mac, err := ethernet.MakeMAC(macStr)
	if err == nil {
		Dump_PcapSta(w, mac)
	} else {
		fmt.Fprintf(w, "Invalid Mac %v\n", macStr)
	}
}

func httpServer(addr string, mux http.Handler) {
	retryListenCnt := 0 // Init the Retry open-HTTP socket counter

	for {
		err := http.ListenAndServe(addr, mux) // doesn't exit unless it fails
		fmt.Printf("Error: can't listen at %q: %s\n", addr, err)

		if strings.Contains(err.Error(), "address already in use") {
			// Retry openning Socket up to Specified number of times
			if retryListenCnt < SocketOpenRetryCnt {
				retryListenCnt++
				time.Sleep(time.Second) // Wait 1 second and Try again
				fmt.Printf("Retry(%v) Opening Socket\n", retryListenCnt)
				continue
			}
			fmt.Printf("ERROR: Failed to open web socket after %v tries. Exiting Now.\n", SocketOpenRetryCnt)
		}

		os.Exit(3) // Exit Pace so wrapper can re-start it
	}

}

func main() {

	fmt.Printf("Start\n")

	flag.Parse()

	mux := http.NewServeMux()

	mux.Handle("/debug/pcap/pcapsta", http.HandlerFunc(DumpPcapSta))

	addr := "127.0.0.1:" + strconv.Itoa(*http_port)

	httpServer(addr, mux)
	for {
	}
}
