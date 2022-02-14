// using https://www.lighthouse3d.com/tutorials/maths/ray-sphere-intersection/
    Intersection_t* calcul_intersection(Sphere_t shpere, Rayon_t ray) // Return an object Intersection representing the intersection of the ray with the current object
    {
        // check if pc is not behind the origin of the ray
        Vector_t v = *sphere->centre - ray.origine;  // %%%% VERIFIER QUE LE - FONCTIONNE SUR LES POSITIONS
        double distance = v ^ ray.direction;
        if (distance <= 0) {return NULL;}  // %%%% VERIFIER QUE LE PRODUIT SCALAIRE * FONCTIONNE

        // find pc the projection of the center on the ray
        Vector_t pc = ray.origine + ray.direction * distance; // %%% VERIF QUE DOUBLE * VECTOR FONCTIONNE OU IMPLEMENTER PROJ

        // check the distance d between pc and the center
        double d = (pc - *sphere->centre).norme();
        // if d > r then no intersection
        if (d > sphere->radius) {return NULL;}
        // if d == r then intersection = pc
        else if (d == sphere->radius) {
            Vector_t normale = pc - *sphere->centre;
            Intersection_t i(distance, normale, *sphere);
            return &i;
        }
        // if d < r then 2 intersections, find the closest and its position
        else {
            double c = sqrt(pow(sphere->radius, 2) - pow(d, 2)); // pythagorean theorem
            double di1 = distance - c;

            Vector_t i1 = ray.origine + ray.direction * di1;
            Vector_t normale = i1 - *sphere->centre;
            Intersection_t i(di1, normale, *sphere);
            return &i;
        }

    }