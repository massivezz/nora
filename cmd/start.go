package cmd

import (
	"fmt"
	"log"
	"runtime"

	"github.com/cratonica/trayhost"
	"github.com/massivezz/nora/server"

	"github.com/spf13/cobra"
)

// startCmd represents the start command
var startCmd = &cobra.Command{
	Use:   "start",
	Short: "",
	Long:  ``,
	Run: func(cmd *cobra.Command, args []string) {
		runtime.LockOSThread()
		srv := server.New(9091)

		go func(srv *server.Server) {
			trayhost.SetUrl("http://localhost:9091")
			if err := srv.Serve(); err != nil {
				log.Fatal(err)
			}
		}(srv)

		trayhost.EnterLoop("nora", iconData)

		fmt.Println("Exiting...")
		srv.Stop()
	},
}

func init() {
	RootCmd.AddCommand(startCmd)
}
