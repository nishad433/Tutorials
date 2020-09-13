package chat

import (
	"bufio"
	"fmt"
	"net"
	"strings"
)

type client struct {
	conn     net.Conn
	cur_room *room
	channel  chan command
	name     string
	reader   *bufio.Reader
	writer   *bufio.Writer
}

func (c *client) init() bool {
	name := strings.ReplaceAll(c.conn.RemoteAddr().String(), ".", "")
	name = strings.ReplaceAll(name, ":", "")
	c.name = "user" + name

	c.reader = bufio.NewReader(c.conn)
	c.writer = bufio.NewWriter(c.conn)

	if c.writer != nil && c.reader != nil {
		c.write(fmt.Sprintf("Welcome \"%s\", For list of Commands, type \"/help\"\n", c.name))
		return true
	}
	return false
}

func (c *client) write(msg string) bool {
	if c.writer != nil {
		var prompt string
		if c.cur_room != nil {
			prompt = c.cur_room.name + ">"
		} else {
			prompt = ">"
		}
		msg = msg + prompt
		_, err := c.writer.WriteString(msg)
		if err == nil {
			return c.writer.Flush() == nil
		}
	}
	return false
}

func (c *client) readInput() {
	for {
		if c.reader == nil {
			return
		}
		data, err := c.reader.ReadString('\n')
		if err != nil {
			return
		}
		data = strings.Trim(data, "\r\n")
		args := strings.Split(data, " ")
		cmd := strings.TrimSpace(args[0])

		switch cmd {

		case "/name":
			c.channel <- command{
				id:     CMD_NAME,
				client: c,
				args:   args[1:],
			}
		case "/join":
			c.channel <- command{
				id:     CMD_JOIN,
				client: c,
				args:   args[1:],
			}
		case "/msg":
			c.channel <- command{
				id:     CMD_MSG,
				client: c,
				args:   args[1:],
			}
			c.write("")

		case "/help":
			c.channel <- command{
				id:     CMD_HELP,
				client: c,
			}
		case "/quit":
			c.channel <- command{
				id:     CMD_QUIT,
				client: c,
			}
		case "/rooms":
			c.channel <- command{
				id:     CMD_ROOMS,
				client: c,
			}
		case "/list":
			c.channel <- command{
				id:     CMD_LIST,
				client: c,
			}
		case "":
			c.write("")
		default:
			if c.cur_room != nil {
				c.channel <- command{
					id:     CMD_MSG,
					client: c,
					args:   args,
				}
			}
			c.write("")

		}

	}
}
