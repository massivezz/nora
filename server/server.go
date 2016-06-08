package server

import (
	"fmt"
	"time"

	"sync"

	"github.com/labstack/echo"
	"github.com/labstack/echo/engine/standard"

	"github.com/tylerb/graceful"
)

var (
	routesMu sync.RWMutex
	routes   = make(map[string]echo.HandlerFunc)
)

func Register(uri string, route echo.HandlerFunc) {
	routesMu.Lock()
	defer routesMu.Unlock()
	if route == nil {
		panic("server: Register route is nil")
	}
	if _, dup := routes[uri]; dup {
		panic("server: Register called twice for route " + uri)
	}
	routes[uri] = route
}

func New(port int) *Server {
	e := echo.New()
	std := standard.New(":9091")
	std.SetHandler(e)

	srv := &graceful.Server{
		Timeout: 5 * time.Second,
		Server:  std.Server,
		Logger:  graceful.DefaultLogger(),
	}
	return &Server{
		srv: srv,
		e:   e,
	}
}

type Server struct {
	srv *graceful.Server
	e   *echo.Echo
}

func (s *Server) Serve() error {
	fmt.Println("hit")
	for uri, route := range routes {
		fmt.Println("registered ", uri)
		s.e.GET(uri, route)
	}
	return s.srv.ListenAndServe()
}

func (s *Server) Stop() {
	s.srv.Stop(5 * time.Second)
}

type Route func(echo.Context) error
