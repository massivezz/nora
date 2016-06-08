package routes

import (
	"net/http"

	"github.com/massivezz/nora/util"

	"github.com/labstack/echo"
	"github.com/massivezz/nora/server"
	"github.com/toqueteos/webbrowser"
)

func init() {
	server.Register("/netflix", openNetflix)
	server.Register("/heartbeat", heartbeat)
	server.Register("/shutdown", shutdown)
}

func openNetflix(c echo.Context) error {
	if err := webbrowser.Open("https://www.netflix.com/"); err != nil {
		return c.String(http.StatusBadRequest, err.Error())
	}
	return c.String(http.StatusOK, "")
}

func heartbeat(c echo.Context) error {
	return c.String(http.StatusOK, "0")
}

func shutdown(c echo.Context) error {
	util.Shutdown()
	return nil
}
