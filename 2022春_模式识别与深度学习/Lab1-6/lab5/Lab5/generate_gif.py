import imageio
import os


def create_gif(image_list, gif_name, duration=0.35):
    frames = []
    for image_name in image_list:
        frames.append(imageio.imread(image_name))
    imageio.mimsave(gif_name, frames, 'GIF', duration=duration)
    return


def main(image_path, gif_name, duration=0.35):
    files = os.listdir(image_path)
    image_list = []
    for file in files:
        path = os.path.join(image_path, file)
        image_list.append(path)
    create_gif(image_list, gif_name, duration)


if __name__ == '__main__':
    main(image_path='./checkpoint/wgan-gp', gif_name='./checkpoint/adam_wgan-gp.gif')
