#ifndef UTILS_H
#define UTILS_H

double rand01();

/*
	const int N_GROUPS = 25;
    glm::mat4 groupsTransform[N_GROUPS];
    
    glm::mat4 treeTransform[N_GROUPS];
    glm::mat4 rockTransform[N_GROUPS];
    glm::mat4 shroomTransform[N_GROUPS];
*/

// Pour le positionnement des groupes, utilisez
// for (int i = 0; i < N_GROUPS; ++i)
// float x = i % 5;
// float y = i / 5;
// glm::vec3(x * (6 + rand01() * 6) - 25, -1, y * (6 + rand01() * 6) - 25) );


// Pour les input de souris
// int x=0, y=0;
// w.getMouseMotion(x, y);

// Pour le clavier
// w.getKeyHold(Window::Key::W)

#endif // UTILS_H