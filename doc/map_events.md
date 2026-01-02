# Map Events

Map events are stored in a (singly/doubly) linked list with the head (`map.events`) being the most recent one. Once events are consumed they are removed from the list and freed.
