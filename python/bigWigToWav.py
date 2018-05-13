#! /usr/bin/env python
import argparse, sys, os, errno
import logging
logging.basicConfig(level=logging.DEBUG, format='[%(asctime)s] [%(levelname)s] %(name)s: %(message)s')
logger = logging.getLogger('bigWigToWav')

def modulate(values, wav_file, sample_rate=44100, n_channels=2, max_amp=32767, signal_freq=20):
    import numpy as np
    import wave
    from scipy.ndimage import zoom

    upsample_rate = float(sample_rate)/signal_freq
    T = float(len(values))/signal_freq
    #n_samples = int(sample_rate*T)
    x = zoom(values, upsample_rate)
    n_samples = x.shape[0]
    t = np.linspace(0, T, n_samples)
    y = max_amp*np.sin(2*880*np.pi*t)
    y *= x
    y *= float(max_amp)/np.abs(y.max())
    data = np.empty(n_samples*n_channels, dtype='int16')
    channel_index = np.arange(0, n_samples*n_channels, n_channels)
    data[channel_index] = y
    data[channel_index + 1] = data[channel_index]

    wav = wave.open(wav_file, 'wb')
    wav.setnchannels(n_channels)
    wav.setsampwidth(2)
    wav.setframerate(sample_rate)
    wav.setnframes(n_samples)
    wav.setcomptype('NONE', 'no compression')
    wav.writeframes(np.getbuffer(data))
    wav.close()

def bigwig_to_wav(args):
    import numpy as np
    from bx.bbi.bigwig_file import BigWigFile
    from scipy.signal import convolve
    from scipy.stats import norm
    from scipy.ndimage import zoom

    logger.info('read input BigWigfile: ' + args.bigwig_file)
    f_bigwig  = open(args.bigwig_file, 'rb')
    logger.info('read input BED file: ' + args.bed_file)
    f_bed = open(args.bed_file, 'r')
    bigwig = BigWigFile(f_bigwig)

    smooth_filter = None
    scale_factors = None
    if args.smooth == 'boxcar':
        smooth_filter = np.ones(args.window_size, dtype=np.float32)
    elif args.smooth == 'gaussian':
        smooth_filter = norm.pdf(np.linspace(-3, 3, args.window_size*3, endpoint=True)).astype(np.float32)
    if args.smooth != 'none':
        scale_factors = convolve(np.ones(smooth_filter.shape[0]), smooth_filter, mode='same')
    
    if not os.path.exists(args.output_dir):
        logger.info('create output directory: ' + args.output_dir)
        os.makedirs(args.output_dir)

    for line in f_bed:
        c = line.strip().split('\t')
        chrom = c[0]
        start = int(c[1])
        end = int(c[2])
        x = np.nan_to_num(bigwig.get_as_array(chrom, start, end))
        # zoom the signals
        x = zoom(x, args.zoom)
        if args.smooth != 'none':
            # smooth the raw signal with a moving window
            x = convolve(x, smooth_filter, mode='same')
            # scale the signal
            filter_length = smooth_filter.shape[0]
            x[:(filter_length/2)] /= scale_factors[:(filter_length/2)]
            x[(-filter_length/2):] /= scale_factors[(-filter_length/2):]
            if x.shape[0] > filter_length:
                x[(filter_length/2):(-filter_length/2)] /= np.sum(smooth_filter)

        wav_file = os.path.join(args.output_dir, '%s:%d-%d.wav'%(chrom, start, end))
        logger.info('create wav file: ' + wav_file)
        modulate(x, wav_file, sample_rate=args.sample_rate, n_channels=args.n_channels)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Convert BigWig files to WAV audio files')
    parser.add_argument('--bigwig-file', '-i', type=str, required=True, help='input BigWig file')
    parser.add_argument('--bed-file', '-b', type=str, required=True, help='input BED file')
    parser.add_argument('--output-dir', '-o', type=str, required=True, help='output directory')
    parser.add_argument('--sample-rate', '-r', type=int, default=44100)
    parser.add_argument('--n-channels', '-c', type=int, default=2)
    parser.add_argument('--smooth', type=str,
         choices=('gaussian', 'boxcar', 'none'), default='none',
         help='apply a filter to the raw signal')
    parser.add_argument('--window-size', '-w', type=int, default=5)
    parser.add_argument('--signal-freq', type=int, default=100)
    parser.add_argument('--zoom', '-z', type=float, default=1, help='zoom factor')

    args = parser.parse_args()

    bigwig_to_wav(args)