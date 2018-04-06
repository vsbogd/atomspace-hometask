#include <ctime>
#include <opencog/atomspace/AtomSpace.h>
#include <opencog/query/BindLinkAPI.h>

using namespace opencog;

AtomSpace atomSpace;

#define N atomSpace.add_node
#define L atomSpace.add_link

#define BB_COUNT     256
#define BB_PER_FRAME 16

int main()
{
    std::string objects[10] = {"apple", "cup", "hat", "face", "note", "book", "door", "wall", "lamp", "arm"};

    for(int frame = 0; frame < BB_COUNT / BB_PER_FRAME; frame++) {
        std::string frameName = "Frame-" + std::to_string(frame);

        for(int boundingBox = 0; boundingBox < BB_PER_FRAME; boundingBox++) {
            std::string boundingBoxName = "BB-" + std::to_string(frame) + "-" + std::to_string(boundingBox);

            L(MEMBER_LINK, N(NODE, frameName), N(NODE, boundingBoxName));
            L(INHERITANCE_LINK, N(NODE, boundingBoxName), N(CONCEPT_NODE, objects[rand()%10]));
            L(MEMBER_LINK, L(INHERITANCE_LINK, N(NUMBER_NODE, std::to_string(rand()%1000)), N(CONCEPT_NODE, "Left")), N(NODE, boundingBoxName));
            L(MEMBER_LINK, L(INHERITANCE_LINK, N(NUMBER_NODE, std::to_string(rand()%1000)), N(CONCEPT_NODE, "Top")), N(NODE, boundingBoxName));
            L(MEMBER_LINK, L(INHERITANCE_LINK, N(NUMBER_NODE, std::to_string(rand()%1000)), N(CONCEPT_NODE, "Right")), N(NODE, boundingBoxName));
            L(MEMBER_LINK, L(INHERITANCE_LINK, N(NUMBER_NODE, std::to_string(rand()%1000)), N(CONCEPT_NODE, "Bottom")), N(NODE, boundingBoxName));
        }
    }

    Handle bindLink =
        L(BIND_LINK,
                L(VARIABLE_LIST,
                    N(VARIABLE_NODE, "$UpperBB"),
                    N(VARIABLE_NODE, "$LowerBB"),
                    N(VARIABLE_NODE, "$Frame"),
                    N(VARIABLE_NODE, "$Top"),
                    N(VARIABLE_NODE, "$Bottom")
                 ),
                L(AND_LINK,
                    L(MEMBER_LINK, N(VARIABLE_NODE, "$Frame"), N(VARIABLE_NODE, "$UpperBB")),
                    L(MEMBER_LINK, N(VARIABLE_NODE, "$Frame"), N(VARIABLE_NODE, "$LowerBB")),
                    L(MEMBER_LINK, L(INHERITANCE_LINK, N(VARIABLE_NODE, "$Top"), N(CONCEPT_NODE, "Top")), N(VARIABLE_NODE, "$LowerBB")),
                    L(MEMBER_LINK, L(INHERITANCE_LINK, N(VARIABLE_NODE, "$Bottom"), N(CONCEPT_NODE, "Bottom")), N(VARIABLE_NODE, "$UpperBB")),
                    L(GREATER_THAN_LINK, N(VARIABLE_NODE, "$Top"), N(VARIABLE_NODE, "$Bottom"))
                 ),
                L(EVALUATION_LINK,
                    N(PREDICATE_NODE, "Higher-Than"),
                    L(LIST_LINK,
                        N(VARIABLE_NODE, "$UpperBB"),
                        N(VARIABLE_NODE, "$LowerBB")
                     )
                 )
         );


    clock_t start = clock();
    Handle result = bindlink(&atomSpace, bindLink);
    clock_t time = clock() - start;

    std::cout << "matching finished in " << (double) time / CLOCKS_PER_SEC << " seconds (" << time << " clocks)" << std::endl;
    std::cout << "matching result: " << result->to_string() << std::endl;

    return 0;
}
