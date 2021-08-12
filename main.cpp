#include "glhelper.h"
#include <pipeline.h>

std::shared_ptr<GLHelper> glhelper_;

void loop(int)
{
    glhelper_->wait_refresh();
    glutTimerFunc(0, loop, 0);
};

static void reshape(int w, int h)
{
    glhelper_->resize_gl(w, h);
}

static void display()
{
    glhelper_->wait_refresh();
}

int main(int argc, char **argv)
{
    setenv("MWV206_GLVERSION", "1", 1);
    glutInit(&argc, argv);

    glhelper_ = std::make_shared<GLHelper>();

    Pipeline pipe(argv[1], glhelper_);

    pipe.start();

    // std::vector<gl_item_info> item_info;
    // item_info.push_back({checkImageWidth, checkImageHeight, -1, -1, 1, 1, TEX_PTR});
    // item_info.push_back({checkImageWidth, checkImageHeight, -0.5, -0.5, 0.25, 0.25, TEX_PTR});
    // item_info.push_back({checkImageWidth, checkImageHeight, -0.5, -0.5, 0.25, 0.25, TEX_PTR});
    // makeCheckImages();

    // glhelper_->register_item(item_info.size(), item_info);
    // glhelper_->update_tex(0, otherImage);
    // glhelper_->update_tex(1, otherImage2);
    // glhelper_->update_tex(2, otherImage3);


    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutTimerFunc(0, loop, 0);
    glutMainLoop();
    return 0;
}