#include <cstdio>
#include <string>
#include <enet/enet.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: gacha64server {port_number}\n");
        return -1;
    }

    std::string portString(argv[1]);

    int port;
    try {
        port = std::stoi(portString);
    } catch (const std::exception &ex) {
        printf("Invalid port: %s", portString.c_str());
        return -2;
    }

    ENetAddress address;
    ENetHost *server;

    address.host = ENET_HOST_ANY;
    address.port = port;

    server = enet_host_create(&address, 3, 2, 0, 0);
    if (!server) {
        printf("Error creating server.");
        return -1;
    }

    printf("Listening on port %d", port);
    fflush(stdout);

    ENetEvent event;
    bool exit = false;
    while (!exit) {
        /* Wait up to 1000 milliseconds for an event. */
        while (enet_host_service(server, &event, 1000)) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    printf("\nA new client connected from %x:%u.\n",
                           event.peer->address.host,
                           event.peer->address.port);

                    break;
                case ENET_EVENT_TYPE_RECEIVE: {
                    printf("\nA packet of length %zu containing %s was received from %u on channel %u.\n",
                           event.packet->dataLength,
                           event.packet->data,
                           event.peer->connectID,
                           event.channelID);
                    /* Clean up the packet now that we're done using it. */
                    enet_packet_destroy(event.packet);

                    std::string result("Received packet!");
                    ENetPacket *packet = enet_packet_create(result.c_str(), result.length(), 0);
                    enet_host_broadcast(server, 0, packet);
                }
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    printf("\n%u disconnected.\n", event.peer->connectID);
                    break;
                case 4:
                    exit = true;
                    break;
            }
        }
    }

    enet_host_destroy(server);
    return 0;
}
