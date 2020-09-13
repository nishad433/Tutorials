package chat

import (
	"fmt"
	"log"
	"net"
	"strings"
)

type commandID int

const (
	CMD_NAME commandID = iota
	CMD_JOIN
	CMD_MSG
	CMD_QUIT
	CMD_ROOMS
	CMD_HELP
	CMD_LIST
)

type command struct {
	id     commandID
	client *client
	args   []string
}

type room struct {
	name    string
	members map[net.Addr]*client
}

func (r *room) broadcast(sender *client, msg string) {

	for addr, c := range r.members {
		if addr != sender.conn.RemoteAddr() {
			c.write(msg)
		}

	}
}

type server struct {
	rooms   map[string]*room
	channel chan command
}

func NewServer() *server {

	s := &server{
		rooms:   make(map[string]*room),
		channel: make(chan command),
	}

	return s
}

func (s *server) NewClient(conn net.Conn) {

	log.Printf("New Client connected from %s\n", conn.RemoteAddr().String())
	c := &client{
		conn:    conn,
		channel: s.channel,
	}

	c.init()
	c.readInput()

}

func (s *server) Run() {

	for {
		cmd := <-s.channel

		switch cmd.id {

		case CMD_NAME:
			s.name(cmd.client, cmd.args)
		case CMD_QUIT:
			s.quit(cmd.client)
		case CMD_HELP:
			s.help(cmd.client)
		case CMD_ROOMS:
			s.listrooms(cmd.client)
		case CMD_JOIN:
			s.join(cmd.client, cmd.args)
		case CMD_MSG:
			s.msg(cmd.client, cmd.args)
		case CMD_LIST:
			s.listMembers(cmd.client)

		}

	}

}

func (s *server) name(c *client, args []string) {
	if len(args) > 0 {
		name := args[0]
		c.write(fmt.Sprintf("All right, you are %s\n", name))
		if c.cur_room != nil {
			c.cur_room.broadcast(c, fmt.Sprintf("%s changed name to %s\n", c.name, name))
		}
		c.name = name
	} else {
		c.write(fmt.Sprintf("You are %s\n", c.name))
	}
}
func (s *server) quit(c *client) {
	if !s.quitCurrRoom(c) {
		c.write(fmt.Sprintf("Thank You %s.\n", c.name))
		log.Printf("Closed connection to Client %s\n", c.conn.RemoteAddr().String())
		c.conn.Close()
	}
}
func (s *server) help(c *client) {
	var usageTemplate = `
/name <Name>	    - Display/Update Name
/join <Room Name>   - Create/Join a Chatroom
/msg  <msg>	    - Send msg to a Chatroom
/rooms		    - List available Chatrooms
/quit		    - Exit a Chatroom/Server
/help		    - List available Commands
`
	c.write(usageTemplate)
}
func (s *server) listrooms(c *client) {
	var roomsSlice []string
	for roomName, _ := range s.rooms {
		roomsSlice = append(roomsSlice, roomName)
	}
	c.write(fmt.Sprintf("Available Rooms:%s\n", strings.Join(roomsSlice, ",")))
}
func (s *server) join(c *client, args []string) {
	if len(args) != 1 {
		c.write("Empty/Invalid Room Name\n")
	}

	roomName := strings.ToUpper(args[0])
	r, err := s.rooms[roomName]
	if !err {
		r = &room{
			name:    roomName,
			members: make(map[net.Addr]*client),
		}
		s.rooms[roomName] = r
	}

	s.quitCurrRoom(c)
	c.cur_room = r
	r.members[c.conn.RemoteAddr()] = c

	c.cur_room.broadcast(c, fmt.Sprintf("%s joined the room\n", c.name))
	c.write(fmt.Sprintf("Joined Room %s\n", r.name))

}
func (s *server) msg(c *client, args []string) {

	if c.cur_room != nil {
		c.cur_room.broadcast(c, fmt.Sprintf("[%s:] %s\n", c.name, strings.Join(args, " ")))
	}
}

func (s *server) listMembers(c *client) {
	if c.cur_room != nil {
		var members []string
		for _, c := range c.cur_room.members {
			members = append(members, c.name)
		}
		members_str := strings.Join(members, "\n")
		c.write(fmt.Sprintf("Members of Chatroom [%s]\n%s\n\n", c.cur_room.name, members_str))

	} else {
		c.write("Not part of any Chatroom\n")
	}
}

func (s *server) quitCurrRoom(c *client) bool {
	if c.cur_room != nil {
		oldRoom := c.cur_room
		delete(oldRoom.members, c.conn.RemoteAddr())
		c.cur_room = nil
		c.write(fmt.Sprintf("Left %s.\n", oldRoom.name))
		oldRoom.broadcast(c, fmt.Sprintf("%s left the room\n", c.name))
		return true
	}
	return false

}
